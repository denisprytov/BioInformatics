#include <iostream>
#include <string>
#include <vector> 
#include <map>
#include <algorithm>
using namespace std;
int main()
{
	map<char, int> table_mass_amino_acid = { {'G', 57},{'A', 71}, {'S', 87}, {'P', 97}, {'V', 99},	{'T', 101},	{'C', 103},	{'I', 113},	{'L', 113},	{'N', 114},	{'D', 115},	{'K', 128},
										     {'Q', 128}, {'E', 129}, {'M', 131}, {'H', 137}, {'F', 147}, {'R', 156}, {'Y', 163},	{'W', 186} };
	string peptide;
	cin >> peptide;
	string tmp_peptide = peptide;
	tmp_peptide += peptide;
	vector<int> fragment_masses;

	fragment_masses.push_back(0);

	for (size_t i = 0; i < peptide.size(); ++i)
	{
		for (size_t j = 1; j < peptide.size(); ++j)
		{
			int mass = 0;
			string part = tmp_peptide.substr(i, j);

			for (size_t k = 0; k < part.size(); ++k)
			{
				mass += table_mass_amino_acid.at(part[k]);
			}

			fragment_masses.push_back(mass);
		}
	}

	int mass_p = 0;
	for (size_t k = 0; k < peptide.size(); ++k)
	{
		mass_p += table_mass_amino_acid.at(peptide[k]);
	}

	fragment_masses.push_back(mass_p);

	sort(fragment_masses.begin(), fragment_masses.end());

	for(auto& mass : fragment_masses)
	{
		cout << mass << " ";
	}
	return 0;
}