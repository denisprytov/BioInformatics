#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
using namespace std;

/**
 * \brief массы аминокислот
 */
const map<char, int> amino_acid_masses = { {'G', 57}, { 'A', 71 }, { 'S', 87 }, { 'P', 97 }, { 'V', 99 },
										   { 'T', 101 }, { 'C', 103 }, { 'I', 113 }, { 'L', 113 }, { 'N', 114 },
										   { 'D', 115 }, { 'K', 128 }, { 'Q', 128 }, { 'E', 129 }, { 'M', 131 },
										   { 'H', 137 }, { 'F', 147 }, { 'R', 156 }, { 'Y', 163 }, { 'W', 186 } };

/**
 * \brief функция генерирующая теоретический спекрт циклического пептида
 * \param peptide пептид
 * \return вектор масс теоретического спектра
 */
vector<string> cyclospectrum(string peptide) {
	vector<string> spectrum_masses { "0" };
	string tmp = peptide + peptide;
	for (size_t i = 0; i < peptide.size(); ++i)
		for (size_t j = 1; j < peptide.size(); ++j) {
			int mass = 0;
			string part = tmp.substr(i, j);
			for (auto k : part) {
				mass += amino_acid_masses.at(k);
			}
			spectrum_masses.push_back(to_string(mass));
		}
	int mass_peptide = 0;
	for (auto amino_acid : peptide) {
		mass_peptide += amino_acid_masses.at(amino_acid);
	}
	spectrum_masses.push_back(to_string(mass_peptide));
	sort(spectrum_masses.begin(), spectrum_masses.end());
	return spectrum_masses;
}

/**
 * \brief функция парсинга спектра
 * \param spectrum спекртр
 * \return список масс экспериментального спектра
 */
list<string> parse_to_list(const string& spectrum) {
	list<string> cyclospectrum_masses;
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
	return cyclospectrum_masses;
}

/**
 * \brief scoring function
 * \param peptide пептид
 * \param spectrum спектр
 * \return количество общих масс
 */
int score(const string& peptide, const string& spectrum) {
	list<string> spectrum_masses = parse_to_list(spectrum);
	vector<string> theoretical_spectrum = cyclospectrum(peptide);
	int score = 0;
	for (auto& mass : theoretical_spectrum) {
		if (find(spectrum_masses.begin(), spectrum_masses.end(), mass) != spectrum_masses.end()) {
			spectrum_masses.erase(find(spectrum_masses.begin(), spectrum_masses.end(), mass));
			score++;
		}
	}
	return score;
}

/**
 * \brief функция main, реализующая подсчет общих масс между cyclospectrum(peptide) и spectrum
 * \details Для обобщения проблемы секвенирования циклопептидов для обработки 
 * зашумленных спектров, необходимо отменить требование, чтобы 
 * теоретический спектр кандидата пептида точно соответствовал
 * экспериментальному спектру
 * Введем функцию подсчёта, которая будет выбирать пептид, теоретический 
 * спектр которого лучше всего соответствует данному экспериментальному 
 * спектру:
 * Для заданного циклического пептида peptide и спектра spectrum
 * определим score(peptide, spectrum) как количество общих масс между 
 * cyclospectrum(peptide) и spectrum
 */
int main() {
	string peptide = "NQEL";
	string spectrum = "0 99 114 128 227 242 257 299 355 356 370 371 484";
	getline(cin, peptide);
	getline(cin, spectrum);
	cout << score(peptide, spectrum);
	return 0;
}
