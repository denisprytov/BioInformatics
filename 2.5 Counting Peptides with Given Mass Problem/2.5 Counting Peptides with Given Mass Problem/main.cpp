#include <iostream>
#include <map>

int main()
{
	int amino_acid_masses[18] = { 57, 71, 87, 97, 99, 101, 103, 113, 114, 115, 128, 129, 131, 137, 147, 156, 163, 186 };

	int mass;
	std::cin >> mass;

	std::map<int, uint64_t> num_peptides = { {0, 1} };

	for (int i = 57; i <= mass; ++i)
	{
		num_peptides[i] = 0;

		for (int j = 0; j < 18; ++j)
		{
			if (num_peptides.find(i - amino_acid_masses[j]) != num_peptides.end())
			{
				num_peptides[i] += num_peptides[i - amino_acid_masses[j]];
			}
		}
	}

	std::cout << num_peptides[mass];
	return 0;
}
