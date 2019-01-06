#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <set>
using namespace std;

/**
 * \brief генерируем все возможные k-меры из данных строк dna
 * \param dna дезоксирибонуклеиновая кислота
 * \param k integer
 * \return вектор k-меров
 */
vector<string> get_all_k_mers_from_dna(const vector<string>& dna, int k) {
	vector<string> k_mers;
	for (auto& dna_string : dna) {
		for (auto i = 0; i < dna_string.size() - k + 1; ++i) {
			k_mers.push_back(dna_string.substr(i, k));
		}
	}
	return k_mers;
}

/**
 * \brief проверяем отличие не более чем на d мутаций
 * \param word слово
 * \param k_mer_pattern k-мер
 * \param d число мутаций
 */
bool check_mismatches(const string& word, const string& k_mer_pattern, int d) {
	int count = 0;
	for (int i = 0; i < word.size(); ++i) {
		if (word[i] != k_mer_pattern[i]) {
			++count;
		}
		if (count > d) return false;
	}
	return true;
}

/**
 * \brief метод генерации всевозможных слов днины n из даданного алфавита
 * \param length длина слова
 * \param words вектор сгенерированных слов длины n
 */
void gen(size_t length, vector<string>& words) {
	const char alphabet[] = "AGCT";
	const size_t last_digit = sizeof(alphabet) - 2;
	vector<size_t> number(length, 0);
	for (; ; ) {
		string tmp;
		for (auto i : number) {
			tmp += alphabet[i];
		}
		words.push_back(tmp);
		size_t pos = length;
		while (number[--pos] == last_digit) {
			if (pos == 0)
				return;
			number[pos] = 0;
		}
		++number[pos];
	}
}

/**
 * \brief метод генерации всевозможных регуляторных мотивов
 * \param k_mer_pattern k-мер
 * \param d число мутаций
 * \return вектор (k, d)-мотивов
 */
vector<string> get_all_kd_motifs(const string& k_mer_pattern, int d) {
	vector<string> words;
	int test = 0;
	gen(k_mer_pattern.size(), words);
	vector<string> kd_motifs;
	for (auto& word : words) {
		if (check_mismatches(word, k_mer_pattern, d)) {
			kd_motifs.push_back(word);
			test++;
		}
	}
	return kd_motifs;
}

/**
 * \brief ищем k,d-мотив в каждой строке с не более чем d мутациями
 * \param kd_motif (k, d)-мотив
 * \param dna дезоксирибонуклеиновая кислота
 * \param k длина
 * \param d число мутаций
 */
bool is_kd_motif_appear_in_each_dna_string_at_most_d_mismatches(const string& kd_motif, const vector<string>& dna, int k, int d) {
	size_t count = 0;
	for (const auto& dna_string : dna) {
		for (auto i = 0; i < dna_string.size() - k + 1; ++i) {
			if (check_mismatches(dna_string.substr(i, k), kd_motif, d)) {
				++count;
				break;
			}
		}
	}
	return count == dna.size();
}

int main() {
	string tmp;
	int k, d;
	cin >> k;
	cin >> d;
	vector<string> dna;
	while (cin >> tmp) {
		if (tmp == "\n") break;
		dna.push_back(tmp);
	}
	vector<string> patterns;
	vector<string> k_mers = get_all_k_mers_from_dna(dna, k);
	for (auto& k_mer_pattern : k_mers) {
		vector<string> kd_motifs = get_all_kd_motifs(k_mer_pattern, d);
		for (auto& kd_motif : kd_motifs) {
			if (is_kd_motif_appear_in_each_dna_string_at_most_d_mismatches(kd_motif, dna, k, d)) {
				patterns.push_back(kd_motif);
			}
		}
	}
	set<string> kd_motifs_set(patterns.begin(), patterns.end());
	string output;
	for (auto elem : kd_motifs_set) {
		output += elem + " ";
	}
	output.erase(output.end() - 1);
	cout << output;
	return 0;
}
