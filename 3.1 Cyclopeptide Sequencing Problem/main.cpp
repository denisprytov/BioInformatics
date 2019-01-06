#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

/**
 * \brief ����� �����������
 */
const map<char, int> amino_acid_masses = { {'G', 57}, { 'A', 71 }, { 'S', 87 }, { 'P', 97 }, { 'V', 99 },
										   { 'T', 101 }, { 'C', 103 }, { 'I', 113 }, { 'L', 113 }, { 'N', 114 },
										   { 'D', 115 }, { 'K', 128 }, { 'Q', 128 }, { 'E', 129 }, { 'M', 131 },
										   { 'H', 137 }, { 'F', 147 }, { 'R', 156 }, { 'Y', 163 }, { 'W', 186 } };

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
 * \brief �������� �������������� ������� ������������ �������
 * \param peptide ������
 * \return ������������� ������
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
 * \brief ���������� ����������� �� ������� ��������� � ������������ �������
 * \details ��� ��������� ������������������ ������� spectrum ������������ �������,
 * �������� ������ ����������� �� spectrum , ���� ������ ����� � ���
 * ������������� ������� ���������� � spectrum.
 * \param peptide ������
 * \param spectrum ����������� ������
 * \return ���� ����������� - true, ����� false
 */
bool peptide_consistent(const string& peptide, const string& spectrum) {
	vector<string> cyclospectrum_masses;
	vector<string> linearspec_masses = { "0" };
	::linearspec_masses(peptide, linearspec_masses); /// �������������� �������� ������ ��� ��������� �������
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
 * \brief ������� main, ����������� branch and bound �������� cyclopeptide sequencing
 * \details ����: ������ ����, ����� ��������� ��� ����������� ������� � �������� ������,
 * ����� ������������ ������������� �������� �������, ������������� �������
 * ������� ����������� � ����������������� ��������
 * ��� ��������� ������������������ ������� spectrum ���������� ����� peptides
 * ������������� �������� ��������:
 * ������������� peptides ������� �� ������� �������, ������� ������������ �����
 * ������ ������ ������ (������������ "") c ������ 0.
 * �� ��������� ���� ����� ��������� peptides, ����� ��� ��������� ���
 * �������� ������� ������ 1.
 * ��������� ���� �������, �������� 18 ����� �������� ������ k + 1 ��� ������
 * �������������� ������������������ peptide ������ k � peptides, �������� ���
 * ��������� �������������� ����� � ����� peptide.
 * ���������, ����� �� �����-���� �� ���� ����� �������� �������� �����, ������
 * parent_mass(spectrum), ���� ��, ������� ��� � �����.
 * ����� ���������� ��������-���������� �� ������������� ���������������,
 * ������ ���, ����� ����������� peptides, ����� �������� ������ ������,
 * �������� ������ �� �������� �������, ������� ����������� � ����������������� ��������.
 */
int main() {
	list<string> peptides = { "" }; /// ������ ��������
	list<string> output; /// �������� ������
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