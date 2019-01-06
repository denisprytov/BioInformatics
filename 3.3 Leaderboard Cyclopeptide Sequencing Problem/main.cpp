#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

/**
 * \brief ����� �����������
 */
const map<char, int> amino_acid_masses = { {'G', 57}, { 'A', 71 }, { 'S', 87 }, { 'P', 97 }, { 'V', 99 },
										   { 'T', 101 }, { 'C', 103 }, { 'I', 113 }, { 'L', 113 }, { 'N', 114 },
										   { 'D', 115 }, { 'K', 128 }, { 'Q', 128 }, { 'E', 129 }, { 'M', 131 },
										   { 'H', 137 }, { 'F', 147 }, { 'R', 156 }, { 'Y', 163 }, { 'W', 186 } };

/**
 * \brief ������� �������� �������
 * \param spectrum ������
 * \return ������ ���� ������������������ �������
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
 * \brief ���������� ����������� ����� � �������
 * \param spectrum ������
 * \return ���������� ����� � spectrum
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
 * \brief ������������ ����� �������
 * \param peptide ������, ����� �������� �� ����� �����
 * \return ����� �������
 */
int mass(const string& peptide) {
	int mass = 0;
	for (auto& amino_acid : peptide) {
		mass += amino_acid_masses.at(amino_acid);
	}
	return mass;
}

/**
 * \brief ��������� ������ ���������
 * \param peptides ������ ��������
 * \details ������� ����� ������� ������ k + 1 ��� ������
 * �������������� ������������������ peptide ������ k � peptides, �������� ���
 * ��������� �������������� ����� � ����� peptide
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
 * \brief ���������� ������������� �������� ������ �������
 * \param peptide ������
 * \param linearspec_masses ������ ����
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
 * \details ������������ ���������� ����� ���� � ������������� ������� ��������� �������
 * � ������ ����������������� �������, �������� ��������������� ����, �.�. ������� ��� 
 * ��� ����������� � ������ �������
 * \param peptide ������
 * \param spectrum ������
 * \return ���������� ����� ����
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
 * \brief ������� ��������� ���� ���������
 * \param left_score
 * \param right_score 
 */
bool compare_scores(const int& left_score, const int& right_score) {
	return left_score > right_score;
}

/**
 * \brief ������� ������������ ������� ������ num �������� �������� � ��������� score
 * \param leader_board ������� �������
 * \param spectrum ������
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
 * \details ������ ����, ����� ��������� ��� ����������� ������� � �������� ������,
 * ����� ������������ ������������� �������� �������, ������������� �������
 * ������� ����������� � ����������������� ��������
 * ��� ��������� ������������������ ������� spectrum ���������� ����� leader_board, 
 * ������� �������� num ���������� � ��������� score ��� ����������� ����������
 * (leader_board ����� ��������� ����� num ���������).
 * ������������� leader_board ������� �� ������� �������, ������� ������������ �����
 * ������ ������ ������ (������������ "") c ������ 0.
 * �� ��������� ���� ����� ��������� leader_board, ����� �� �������� ���
 * �������� ������� ������ 1.
 * ��������� ���� �������, �������� 18 ����� �������� ������ k + 1 ��� ������
 * �������������� ������������������ peptide ������ k � leader_board, �������� ���
 * ��������� �������������� ����� � ����� peptide.
 * ���������, ����� �� �����-���� �� ���� ����� �������� �������� �����, ������
 * parent_mass(spectrum), ���� ��, � �� ����� ���������� score, �������
 * ��� � leader_peptide.
 * ��� ��������� ������ �������� leader_board, ������� spectrum �
 * ������ ����� num, ��������� trim(leader_board, spectrum, num) ��� �����
 * ������ num �������� �������� � ��������� score � leader_board
 * (������� ��������) �� ��������� � spectrum.
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
