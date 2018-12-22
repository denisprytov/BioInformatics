using System;
using System.Collections.Generic;
using System.Linq;

namespace _4._1_Motif_Enumeration_Problem {
    class Program {
        static List<string> words_set(string abc, int len) {
            List<string> words = new List<string>((int)Math.Pow(4, len));
            Program.words(ref words, abc, len, "");
            return words;
        }

        private static void words(ref List<string> words, string abc, int len, string prefix) {
            if (len == 0) {
                words.Add(prefix);
            }
            else
                foreach (char c in abc) {
                    Program.words(ref words, abc, len - 1, prefix + c);
                }
        }

        static bool admit_mismatches(string original, string replica, int d_mismatches) {
            int len = original.Length;
            int mismatches = 0;
            for (int i = 0; i < len; i++) {
                if (original[i] != replica[i]) {
                    mismatches++;
                }
                if (mismatches > d_mismatches) {
                    return false;
                }
            }
            return true;
        }

        static List<string> kd_motifs(string k_mer, int admit_mismatches) {
            string abc = "AGCT";
            List<string> patterns = new List<string>();
            foreach (var item in words_set(abc, k_mer.Length)) {
                if (Program.admit_mismatches(item, k_mer, admit_mismatches))
                {
                    patterns.Add(item);
                }
            }
            return patterns;
        }

        private static void Main(string[] args) {
            string input = Console.ReadLine();
            string[] kd = input.Split(' ');
            var k = int.Parse(kd[0]);
            var d = int.Parse(kd[1]);
            string tmp = "";
            while (true){
                string s = Console.ReadLine();
                if (string.IsNullOrEmpty(s))
                    break;
                tmp += s + ' ';
            }
            string[] dna = tmp.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries).ToArray();
            List<string> patterns = new List<string>();
            foreach (string s in dna) {
                int len = s.Length;
                for (int i = 0; i < len - k + 1; i++) {
                    string k_mer = s.Substring(i, k);
                    foreach (string pattern in kd_motifs(k_mer, d)) {
                        int count = 0;
                        foreach (string substr in dna) {
                            for (int j = 0; j < len - k + 1; j++) {
                                if (admit_mismatches(substr.Substring(j, k), pattern, d)) {
                                    count++;
                                    break;
                                }
                            }
                        }
                        if (count == dna.Length) {
                            patterns.Add(pattern);
                        }
                    }
                }
            }
            patterns = patterns.Distinct().ToList();
            Console.WriteLine(string.Join(" ", patterns));
            Console.ReadKey();
        }
    }
}