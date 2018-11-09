#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

string reverse(string s1) {
	string s2 = "";
	size_t size = s1.size();
	for (size_t i = 0; i < size; i++) {
		switch (s1[i]) {
		case 'A': {
			s1[i] = 'T';
			break;
		}
		case 'C': {
			s1[i] = 'G';
			break;
		}
		case 'G': {
			s1[i] = 'C';
			break;
		}
		case 'T': {
			s1[i] = 'A';
			break;
		}
		default: ;
		}
	}
	for (auto it = (s1.end() - 1); it != (s1.begin() - 1); --it)
		s2 += (*it);
	return s2;
}

int main()
{
	map<string, char> rna_codon_table = { {"AAA", 'K'}, {"AAC", 'N'}, {"AAG", 'K'}, {"AAU", 'N'}, {"ACA", 'T'}, {"ACC", 'T'}, {"ACG", 'T'}, {"ACU", 'T'}, {"AGA", 'R'}, {"AGC", 'S'}, {"AGG", 'R'}, {"AGU", 'S'}, {"AUA", 'I'}, {"AUC", 'I'},
										  {"AUG", 'M'}, {"AUU", 'I'}, {"CAA", 'Q'}, {"CAC", 'H'}, {"CAG", 'Q'}, {"CAU", 'H'}, {"CCA", 'P'}, {"CCC", 'P'}, {"CCG", 'P'}, {"CCU", 'P'}, {"CGA", 'R'}, {"CGC", 'R'}, {"CGG", 'R'}, {"CGU", 'R'},
										  {"CUA", 'L'}, {"CUC", 'L'}, {"CUG", 'L'}, {"CUU", 'L'}, {"GAA", 'E'}, {"GAC", 'D'}, {"GAG", 'E'}, {"GAU", 'D'}, {"GCA", 'A'}, {"GCC", 'A'}, {"GCG", 'A'}, {"GCU", 'A'}, {"GGA", 'G'}, {"GGC", 'G'},
										  {"GGG", 'G'}, {"GGU", 'G'}, {"GUA", 'V'}, {"GUC", 'V'}, {"GUG", 'V'}, {"GUU", 'V'}, {"UAA", ' '}, {"UAC", 'Y'}, {"UAG", ' '}, {"UAU", 'Y'}, {"UCA", 'S'}, {"UCC", 'S'}, {"UCG", 'S'}, {"UCU", 'S'},
										  {"UGA", ' '}, {"UGC", 'C'}, {"UGG", 'W'}, {"UGU", 'C'}, {"UUA", 'L'}, {"UUC", 'F'}, {"UUG", 'L'}, {"UUU", 'F'} 
										};
	string dna_text, peptide;

	vector<string> output;

	cin >> dna_text;
	cin >> peptide;

	for (size_t ind = 0; ind <= dna_text.size() - 3 * peptide.size(); ++ind)
	{
		string rna = "";
		string dna = dna_text.substr(ind, peptide.length() * 3);

		for (size_t i = 0; i < dna.size(); ++i)
		{
			if (dna[i] == 'T')
			{
				rna.push_back('U');
			}
			else
			{
				rna.push_back(dna[i]);
			}
		}

		string rna_reverse = "";

		string dna_reverse = "";
		string s1 = dna_text.substr(ind, peptide.size() * 3);
		size_t size = s1.size();
		for (size_t i = 0; i < size; i++) {
			switch (s1[i]) {
			case 'A': {
				s1[i] = 'T';
				break;
			}
			case 'C': {
				s1[i] = 'G';
				break;
			}
			case 'G': {
				s1[i] = 'C';
				break;
			}
			case 'T': {
				s1[i] = 'A';
				break;
			}
			default:;
			}
		}
		for (auto it = (s1.end() - 1); it != (s1.begin() - 1); --it)
			dna_reverse += (*it);


		for (size_t i = 0; i < dna_reverse.size(); ++i)
		{
			if (dna_reverse[i] == 'T')
			{
				rna_reverse.push_back('U');
			}
			else
			{
				rna_reverse.push_back(dna_reverse[i]);
			}
		}

		string peptide_in_text = "", peptide_in_text_from_reverse_dna = "";
		
		for (size_t i = 0; i < rna.size(); i += 3)
		{
			peptide_in_text += rna_codon_table.at(rna.substr(i, 3));
		}

		for (size_t i = 0; i < rna_reverse.size(); i += 3)
		{
			peptide_in_text_from_reverse_dna += rna_codon_table.at(rna_reverse.substr(i, 3));
		}
		
		if (peptide == peptide_in_text || peptide == peptide_in_text_from_reverse_dna)
		{
			output.push_back(dna_text.substr(ind, peptide.length() * 3));
		}
	}

	for (const auto& tmp : output)
	{
		cout << tmp << endl;
	}
	return 0;
}