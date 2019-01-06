#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

/**
 * \brief массы аминокислот
 */
const map<char, int> amino_acid_masses = { {'G', 57}, { 'A', 71 }, { 'S', 87 }, { 'P', 97 }, { 'V', 99 },
										   { 'T', 101 }, { 'C', 103 }, { 'I', 113 }, { 'L', 113 }, { 'N', 114 },
										   { 'D', 115 }, { 'K', 128 }, { 'Q', 128 }, { 'E', 129 }, { 'M', 131 },
										   { 'H', 137 }, { 'F', 147 }, { 'R', 156 }, { 'Y', 163 }, { 'W', 186 } };

/**
 * \brief функция парсинга спектра
 * \param spectrum спектр
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
 * \brief scoring 
 * \details подсчитываем количество общих масс в теоретическом спектре линейного пептида
 * и данном экспериментальном спектре, учитывая множественность масс, т.е. сколько раз 
 * они встречаются в каждом спектре
 * \param peptide пептид
 * \param spectrum спектр
 * \return количество общих масс
 */
int score(const string& peptide, const string& spectrum) {
	list<string> spectrum_masses = parse_to_list(spectrum);
	vector<string> linear_theoretical_spectrum { "0" };
	if (peptide.size() == 1) linear_theoretical_spectrum.erase(linear_theoretical_spectrum.begin());
	::linearspec_masses(peptide, linear_theoretical_spectrum);
	int score = 0;
	for (auto& mass : linear_theoretical_spectrum) {
		if (find(spectrum_masses.begin(), spectrum_masses.end(), mass) != spectrum_masses.end()) {
			spectrum_masses.erase(find(spectrum_masses.begin(), spectrum_masses.end(), mass));
			score++;
		}
	}
	return score;
}

/**
 * \brief функция сравнения двух элементов
 * \param left_score
 * \param right_score 
 */
bool compare_scores(const int& left_score, const int& right_score) {
	return left_score > right_score;
}

/**
 * \brief функция осуществляет выборку лучших num линейных пептидов с наивысшим score
 * \param leader_board таблица лидеров
 * \param spectrum спектр
 * \param num 
 */
void trim(list<string>& leader_board, const string& spectrum, int num) {
	multimap<int, string, decltype(::compare_scores)*> leader_map(&::compare_scores);
	for (auto& peptide : leader_board) {
		leader_map.insert({ score(peptide, spectrum), peptide });
	}
	leader_board.clear();
	for (auto& elem : leader_map) {
		if (leader_board.size() >= num) {
			if (elem.first == score(leader_board.back(), spectrum)) {
				leader_board.push_back(elem.second);
			continue;
			}
			break;
		}
		leader_board.push_back(elem.second);
	}
}

/**
 * \brief 
 * \details Вместо того, чтобы проверять все циклические пептиды с заданной массой,
 * будем «выращивать» потенциальные линейные пептиды, теоретические спектры
 * которых согласуются с экспериментальным спектром
 * Для заданного экспериментального спектра spectrum сформируем набор leader_board, 
 * который содержит num кандидатов с наивысшим score для дальнейшего расширения
 * (leader_board может содержать более num элементов).
 * Первоначально leader_board состоит из пустого пептида, который представляет собой
 * просто пустую строку (обозначенную "") c массой 0.
 * На следующем шаге будем расширять leader_board, чтобы он содержал все
 * линейные пептиды длиной 1.
 * Продолжим этот процесс, создавая 18 новых пептидов длиной k + 1 для каждой
 * аминокислотной последовательности peptide длиной k в leader_board, добавляя все
 * возможные аминокислотные массы в конец peptide.
 * Проверяем, имеет ли какой-либо из этих новых линейных пептидов массу, равную
 * parent_mass(spectrum), если да, и он имеет наибольший score, заносим
 * его в leader_peptide.
 * Для заданного списка пептидов leader_board, спектра spectrum и
 * целого числа num, определим trim(leader_board, spectrum, num) как набор
 * лучших num линейных пептидов с наивысшим score в leader_board
 * (включая «хвосты») по отношению к spectrum.
 */
int main() {
	int num;
	string spectrum;
	cin >> num;
	cin.ignore();
	getline(cin, spectrum);
	list<string> experimental_spectrum = parse_to_list(spectrum);
	list<string> leader_board = { "" };
	string leader_peptide;
	const auto parent_mass = ::parent_mass(spectrum);
	while (!leader_board.empty()) {
		expand(leader_board);
		list<string> leader_board_copy = leader_board;
		for (auto& peptide : leader_board_copy) {
			if (mass(peptide) == parent_mass) {
				if (score(peptide, spectrum) > score(leader_peptide, spectrum)) {
					leader_peptide = peptide;
				}
			} else
				if (mass(peptide) > parent_mass) leader_board.remove(peptide);
		}
		trim(leader_board, spectrum, num);
	}
	string output;
	for (auto& amino_acid : leader_peptide) {
		output += to_string(amino_acid_masses.at(amino_acid)) + "-";
	}
	output.erase(output.end() - 1);
	cout << output;
	return 0;
}
