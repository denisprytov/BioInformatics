using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3._3_Leaderboard_Cyclopeptide_Sequencing_Problem {
    class Program {
        private static readonly Dictionary<char, int> table_amino_acid_mass = new Dictionary<char, int>() {
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
            List<string> ex_peptides = new List<string>();
            foreach (var peptide in peptides) {
                foreach (var p in table_amino_acid_mass.Keys) {
                    ex_peptides.Add(peptide + p);
                }
            }
            return ex_peptides;
        }
        static string lin_spec(string peptide) {
            if (peptide.Length == 1) return table_amino_acid_mass[peptide[0]].ToString();
            List<int> out_masses = new List<int>() { 0 };
            int mass = 0;
            foreach (var p in peptide) {
                out_masses.Add(table_amino_acid_mass[p]);
                mass += table_amino_acid_mass[p];
            }
            out_masses.Add(mass);
            string cyclo_peptide = peptide + peptide;
            for (int i = 2; i < peptide.Length; i++) {
                for (int j = 0; j < peptide.Length - i; j++) {
                    string sub_peptide = cyclo_peptide.Substring(j, i);
                    int m = 0;
                    foreach (var s in sub_peptide) {
                        m += table_amino_acid_mass[s];
                    }
                    out_masses.Add(m);
                }
            }
            out_masses.Sort();
            return string.Join(" ", out_masses);
        }
        static int score(string peptide, string spectrum) {
            List<string> peptide_masses = lin_spec(peptide).Split(' ').ToList();
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
        static List<string> trim(List<string> leaderboard, string spectrum, int n) {
            leaderboard.Sort((a, b) => score(b, spectrum).CompareTo(score(a, spectrum)));
            if (leaderboard.Count > n) {
                int last = n;
                for (int i = n; i < leaderboard.Count; i++) {
                    if (score(leaderboard[n - 1], spectrum) == score(leaderboard[i], spectrum)) {
                        last = i;
                    }
                    else break;
                }
                leaderboard = leaderboard.Take(last + 1).ToList();
            }
            return leaderboard;
        }
        static void Main(string[] args) {
            string tmp = Console.ReadLine();
            int n = int.Parse(tmp);
            string spectrum = Console.ReadLine();
            int parent_mass = int.Parse(spectrum.Split(' ').Last());
            List<string> leader_board = new List<string>() { "" };
            string leader_peptide = "";
            while (leader_board.Count > 0) {
                leader_board = expand(leader_board);
                List<string> const_peptides = new List<string>(leader_board);
                foreach (var peptide in const_peptides) {
                    if (mass(peptide) == parent_mass) {
                        if (score(peptide, spectrum) > score(leader_peptide, spectrum)) {
                            leader_peptide = peptide;
                        }
                    }
                    else if (mass(peptide) > parent_mass) {
                        leader_board.Remove(peptide);
                    }
                }
                leader_board = trim(leader_board, spectrum, n);
            }
            List<string> output = new List<string>();
            foreach (var p in leader_peptide) {
                output.Add(table_amino_acid_mass[p].ToString());
            }
            Console.WriteLine(string.Join("-", output));
        }
    }
}
