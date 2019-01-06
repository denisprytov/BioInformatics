#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

/**
 * \brief массы аминокислот
 */
const map<char, int> amino_acid_masses = { {'G', 57}, { 'A', 71 }, { 'S', 87 }, { 'P', 97 }, { 'V', 99 },
										   { 'T', 101 }, { 'C', 103 }, { 'I', 113 }, { 'L', 113 }, { 'N', 114 },
										   { 'D', 115 }, { 'K', 128 }, { 'Q', 128 }, { 'E', 129 }, { 'M', 131 },
										   { 'H', 137 }, { 'F', 147 }, { 'R', 156 }, { 'Y', 163 }, { 'W', 186 } };

/**
 * \brief расширяем список перетидов
 * \param peptides список пептидов
 * \details создаем новые пептиды длиной k + 1 для каждой
 * аминокислотной последовательности peptide длиной k в peptides, добавляя все
 * возможные аминокислотные массы в конец peptide
 */
void expand(list<string>& peptides) {
	list<string> peptides_copy = peptides;
	peptides.clear();
	for (auto peptide : peptides_copy) {
		for (auto amino_acid : amino_acid_masses) {
			peptides.push_back(peptide + amino_acid.first);
		}
	}
}

/**
 * \brief рассчитываем массу пептида
 * \param peptide пептид, массу которого мы хотим найти
 * \return масса пептида
 */
int mass(const string& peptide) {
	int mass = 0;
	for (auto& amino_acid : peptide) {
		mass += amino_acid_masses.at(amino_acid);
	}
	return mass;
}

/**
 * \brief вычисление наибольшейй массы в спектре
 * \param spectrum спектр
 * \return наибольшая масса в spectrum
 */
int parent_mass(const string& spectrum) {
	auto iter = spectrum.end() - 1;
	while (*iter == ' ') {
		--iter;
	}
	string mass;
	while (*iter != ' ') {
		string tmp = { *iter };
		tmp += mass;
		mass = tmp;
		--iter;
	}
	return atoi(mass.c_str());
}

/**
 * \brief создание теоретического спектра циклического пептида
 * \param peptide пептид
 * \return теоретический спектр
 */
string cyclospectrum(string peptide) {
	vector<int> spectrum_masses { 0 };
	string tmp = peptide + peptide;
	for (size_t i = 0; i < peptide.size(); ++i)
		for (size_t j = 1; j < peptide.size(); ++j) {
			int mass = 0;
			string part = tmp.substr(i, j);
			for (auto k : part) {
				mass += amino_acid_masses.at(k);
			}
			spectrum_masses.push_back(mass);
		}
	int mass_peptide = 0;
	for (auto amino_acid : peptide) {
		mass_peptide += amino_acid_masses.at(amino_acid);
	}
	spectrum_masses.push_back(mass_peptide);
	sort(spectrum_masses.begin(), spectrum_masses.end());
	string cyclospectrum;
	for (auto mass : spectrum_masses) {
		cyclospectrum += to_string(mass) + " ";
	}
	cyclospectrum.erase(cyclospectrum.end() - 1);
	return cyclospectrum;
}

/**
 * \brief генерируем теоретический линейный спектр пептида
 * \param peptide пептид
 * \param linearspec_masses спектр масс 
 */
void linearspec_masses(const string& peptide, vector<string>& linearspec_masses) {
	for (size_t i = 0; i <= peptide.size(); ++i)
		for (size_t j = 1; j <= peptide.size() - i; ++j) {
			int mass = 0;
			string part = peptide.substr(i, j);
			for (auto k : part) {
				mass += amino_acid_masses.at(k);
			}
			linearspec_masses.push_back(to_string(mass));
		}

	sort(linearspec_masses.begin(), linearspec_masses.end());
}

/**
 * \brief определяем согласуются ли спектры линейного и циклического пептида
 * \details Для заданного экспериментального спектра spectrum циклического пептида,
 * линейный пептид СОГЛАСУЕТСЯ со spectrum , если каждая масса в его
 * теоретическом спектре содержится в spectrum.
 * \param peptide пептид
 * \param spectrum циклический спектр
 * \return если согласуются - true, иначе false
 */
bool peptide_consistent(const string& peptide, const string& spectrum) {
	vector<string> cyclospectrum_masses;
	vector<string> linearspec_masses = { "0" };
	::linearspec_masses(peptide, linearspec_masses); /// инициализируем линейный спектр для задынного пептида
	string tmp;
	for (auto elem : spectrum) {
		if (elem == ' ') {
			cyclospectrum_masses.push_back(tmp);
			tmp = "";
			continue;
		}
		tmp += elem;
	}
	cyclospectrum_masses.push_back(tmp);
	for (auto& mass : linearspec_masses) {
		if (find(cyclospectrum_masses.begin(), cyclospectrum_masses.end(), mass) == cyclospectrum_masses.end()) {
			return false;                                                                                                                                                                                                                               
		}
	}
	return true;
}

/**
 * \brief функция main, реализующая branch and bound алгоритм cyclopeptide sequencing
 * \details Идея: вместо того, чтобы проверять все циклические пептиды с заданной массой,
 * будем «выращивать» потенциальные линейные пептиды, теоретические спектры
 * которых согласуются с экспериментальным спектром
 * Для заданного экспериментального спектра spectrum сформируем набор peptides
 * потенциальных линейных пептидов:
 * Первоначально peptides состоят из пустого пептида, который представляет собой
 * просто пустую строку (обозначенную "") c массой 0.
 * На следующем шаге будем расширять peptides, чтобы они содержали все
 * линейные пептиды длиной 1.
 * Продолжим этот процесс, создавая 18 новых пептидов длиной k + 1 для каждой
 * аминокислотной последовательности peptide длиной k в peptides, добавляя все
 * возможные аминокислотные массы в конец peptide.
 * Проверяем, имеет ли какой-либо из этих новых линейных пептидов массу, равную
 * parent_mass(spectrum), если да, заносим его в ответ.
 * Чтобы количество пептидов-кандидатов не увеличивалось экспоненциально,
 * каждый раз, когда расширяется peptides, будем обрезать данный массив,
 * сохраняя только те линейные пептиды, которые согласуются с экспериментальным спектром.
 */
int main() {
	list<string> peptides = { "" }; /// список пептидов
	list<string> output; /// выходные данные
	string spectrum;
	getline(cin, spectrum);
	const auto parent_mass = ::parent_mass(spectrum);
	while (!peptides.empty()) {
		expand(peptides);
		list<string> peptides_copy = peptides;
		for (string& peptide : peptides_copy) {
			if (mass(peptide) == parent_mass) {
				if (cyclospectrum(peptide) == spectrum) {
					string theoretical_spectrum;
					for (char& amino_acid : peptide) {
						theoretical_spectrum += to_string(amino_acid_masses.at(amino_acid)) + '-';
					}
					theoretical_spectrum.erase(theoretical_spectrum.end() - 1);
					if (find(output.begin(), output.end(), theoretical_spectrum) == output.end()) {
						output.push_back(theoretical_spectrum);
					}
				}
				peptides.remove(peptide);
			} else
				if (!peptide_consistent(peptide, spectrum)) {
					peptides.remove(peptide);
				}
		}
	}
	string out = "";
	for (auto& spec : output) {
		out += spec + " ";
	}
	out.erase(out.end() - 1);
	cout << out;
	return 0;
}