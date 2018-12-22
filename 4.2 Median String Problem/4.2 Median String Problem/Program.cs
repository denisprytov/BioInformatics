using System;
using System.Collections.Generic;
using System.Linq;

namespace _4._2_Median_String_Problem {
    class Program {
        static List<string> words_set(string alphabet, int length) {
            List<string> words = new List<string>((int)Math.Pow(4, length));
            Program.words(ref words, alphabet, length, "");
            return words;
        }

        static void words(ref List<string> words, string alphabet, int length, string prefix) {
            if (length == 0) {
                words.Add(prefix);
            }
            else {
                foreach (char c in alphabet) {
                    Program.words(ref words, alphabet, length - 1, prefix + c);
                }
            }
        }

        static int hamming_distance(string a, string b) {
            int count = 0;
            int length = a.Length;
            for (int i = 0; i < length; i++) {
                if (a[i] != b[i]) {
                    count++;
                }
            }
            return count;
        }

        static int d(string pattern, string text) {
            int length = text.Length;
            int k = pattern.Length;
            List<int> dist = new List<int>();
            for (int i = 0; i < length - k + 1; i++) {
                dist.Add(hamming_distance(pattern, text.Substring(i, k)));
            }
            return dist.Min();
        }

        static int d_dna(string pattern, string[] dna) {
            int sum = 0;
            foreach (string dnai in dna) {
                sum += d(pattern, dnai);
            }
            return sum;
        }

        static string median_string(string[] dna, int k) {
            int distance = int.MaxValue;
            string median = "";
            List<string> k_mers = words_set("AGCT", k);
            foreach (string kmer in k_mers) {
                if (distance > d_dna(kmer, dna))
                {
                    distance = d_dna(kmer, dna);
                    median = kmer;
                }
            }
            return median;
        }

        static void Main(string[] args) {
            int k = int.Parse(Console.ReadLine());
            string buffer = "";
            while (true) {
                string str = Console.ReadLine();
                if (string.IsNullOrEmpty(str))
                    break;
                buffer += str + ' ';
            }
            string[] dna = buffer.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
            Console.WriteLine(median_string(dna, k));
            Console.ReadKey();
        }
    }
}