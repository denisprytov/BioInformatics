using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3._1_Cyclopeptide_Sequencing_Problem {
    class Program {
        static Dictionary<char, int> table_amino_acid_mass = new Dictionary<char, int>() {
                {'G', 57},  {'A', 71},  {'S', 87},  {'P', 97},  {'V', 99},
                {'T', 101}, {'C', 103}, {'I', 113}, {'L', 113}, {'N', 114},
                {'D', 115}, {'K', 128}, {'Q', 128}, {'E', 129}, {'M', 131},
                {'H', 137}, {'F', 147}, {'R', 156}, {'Y', 163}, {'W', 186}
        };
        static int mass(string peptide) {
            int mass = 0;
            foreach (var s in peptide) {
                mass += table_amino_acid_mass[s];
            }
            return mass;
        }
        static List<string> expand(List<string> peptides) {
            List<string> expand_peptides = new List<string>();
            foreach (var peptide in peptides) {
                foreach (var p in table_amino_acid_mass.Keys) {
                    expand_peptides.Add(peptide + p);
                }
            }
            return expand_peptides;
        }
        static string cyclo_spec(string peptide) {
            List<int> masses = new List<int>() { 0 };
            int m = 0;
            foreach (var s in peptide) {
                masses.Add(table_amino_acid_mass[s]);
                m += table_amino_acid_mass[s];
            }
            masses.Add(m);
            string cyclo_pep = peptide + peptide;
            for (int i = 2; i < peptide.Length; i++) {
                for (int j = 0; j < peptide.Length; j++) {
                    string subpeptide = cyclo_pep.Substring(j, i);
                    int tmp = 0;
                    foreach (var s in subpeptide) {
                        tmp += table_amino_acid_mass[s];
                    }
                    masses.Add(tmp);
                }
            }
            masses.Sort();
            return string.Join(" ", masses);
        }
        static string lin_spec(string peptide) {
            if (peptide.Length == 1) return table_amino_acid_mass[peptide[0]].ToString();
            List<int> masses = new List<int>() { 0 };
            int m = 0;
            foreach (var s in peptide) {
                masses.Add(table_amino_acid_mass[s]);
                m += table_amino_acid_mass[s];
            }
            masses.Add(m);
            string cyclo_pep = peptide + peptide;
            for (int i = 2; i < peptide.Length; i++) {
                for (int j = 0; j < peptide.Length - i; j++) {
                    string subpeptide = cyclo_pep.Substring(j, i);
                    int tmp = 0;
                    foreach (var s in subpeptide) {
                        tmp += table_amino_acid_mass[s];
                    }
                    masses.Add(tmp);
                }
            }
            masses.Sort();
            return string.Join(" ", masses);
        }

        static bool consistent(string peptide, string spectrum) {
            List<string> spec_mass = spectrum.Split(' ').ToList();
            List<string> peptideMass = lin_spec(peptide).Split(' ').ToList();
            foreach (var m in peptideMass) {
                if (!spec_mass.Contains(m)) {
                    return false;
                }
            }
            return true;
        }
        static void Main(string[] args) {
            string spectrum = Console.ReadLine(); ;
            int parent_mass = int.Parse(spectrum.Split(' ').Last());
            List<string> peptides = new List<string>() { "" };
            List<string> out_peptides = new List<string>();
            while (peptides.Count > 0) {
                peptides = expand(peptides);
                List<string> const_peptides = new List<string>(peptides);
                foreach (var peptide in const_peptides) {
                    if (mass(peptide) == parent_mass) {
                        if (cyclo_spec(peptide) == spectrum) {
                            out_peptides.Add(peptide);
                        }
                        peptides.Remove(peptide);
                    }
                    else if (!consistent(peptide, spectrum)) {
                        peptides.Remove(peptide);
                    }
                }
            }
            List<string> output = new List<string>();
            foreach (var p in out_peptides) {
                List<string> m = new List<string>();
                foreach (var s in p) {
                    m.Add(table_amino_acid_mass[s].ToString());
                }
                output.Add(string.Join("-", m));
            }
            Console.WriteLine(string.Join(" ", output.Distinct()));
        }
    }
}
