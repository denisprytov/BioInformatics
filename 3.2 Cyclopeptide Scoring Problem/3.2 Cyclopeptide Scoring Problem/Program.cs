using System;
using System.Collections.Generic;
using System.Linq;
namespace _3._2_Cyclopeptide_Scoring_Problem {
    class Program {
        static Dictionary<char, int> table_amino_acid_mass = new Dictionary<char, int>() {
                {'G', 57},  {'A', 71},  {'S', 87},  {'P', 97},  {'V', 99},
                {'T', 101}, {'C', 103}, {'I', 113}, {'L', 113}, {'N', 114},
                {'D', 115}, {'K', 128}, {'Q', 128}, {'E', 129}, {'M', 131},
                {'H', 137}, {'F', 147}, {'R', 156}, {'Y', 163}, {'W', 186}
        };
        static string cyclo_spec(string peptide) {
            List<int> spectrum = new List<int> { 0 };
            int m = 0;
            foreach (var p in peptide) {
                spectrum.Add(table_amino_acid_mass[p]);
                m += table_amino_acid_mass[p];
            }
            spectrum.Add(m);
            string cyclo_peptide = peptide + peptide;
            for (int i = 2; i < peptide.Length; i++) {
                for (int j = 0; j < peptide.Length; j++) {
                    string sub_peptide = cyclo_peptide.Substring(j, i);
                    int curMass = 0;
                    foreach (var p in sub_peptide)
                    {
                        curMass += table_amino_acid_mass[p];
                    }
                    spectrum.Add(curMass);
                }
            }
            spectrum.Sort();
            return string.Join(" ", spectrum);
        }
        static int score(string peptide, string spectrum) {
            List<string> peptide_masses = cyclo_spec(peptide).Split(' ').ToList();
            List<string> spectrum_masses = spectrum.Split(' ').ToList();
            int score = 0;
            foreach (var mass in peptide_masses) {
                if (spectrum_masses.Contains(mass)) {
                    spectrum_masses.Remove(mass);
                    score++;
                }
            }
            return score;
        }
        static void Main(string[] args) {
            string peptide = Console.ReadLine();
            string spectrum = Console.ReadLine();
            int output = score(peptide, spectrum);
            Console.WriteLine(output);
        }
    }
}