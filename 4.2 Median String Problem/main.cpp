#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <set>
using namespace std;

/**
 * \brief метод генерации всевозможных слов днины n из заданного алфавита
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
 * \brief функция подсчитывает число различий между двумя строками
 * \param pattern
 * \param k_mer
 * \return
 */
int hamming_distance(const string& pattern, const string& k_mer) {
	int count = 0;
	for (int i = 0; i < pattern.size(); ++i) {
		if (pattern[i] != k_mer[i]) ++count;
	}
	return count;
}

/**
 * \brief функция вычисляет минимальное расстояние Хэмминга между k_mer pattern и любым k-мером в dna_string
 * \param k_mer
 * \param dna_string строка dna
 */
int d(const string& k_mer, const string& dna_string) {
	set<int> distances;
	for (auto i = 0; i < dna_string.size() - k_mer.size() + 1; ++i) {
		distances.insert(hamming_distance(k_mer, dna_string.substr(i, k_mer.size())));
	}
	return *distances.begin();
}

/**
 * \brief функция подсчитывающая сумму расстояний между k_mer pattern и строками в dna
 * \param k_mer k-мер
 * \param dna
 */
int d(const string& k_mer, const vector<string>& dna) {
	int sum_d = 0;
	for (auto& dna_string : dna) {
		sum_d += d(k_mer, dna_string);
	}
	return sum_d;
}

int main() {
	int k;
	cin >> k;
	string tmp;
	vector<string> dna;
	while (cin >> tmp) {
		if (tmp == "\n") break;
		dna.push_back(tmp);
	}
	getline(cin, tmp);
	string median;
	int distance = INT_MAX;
	vector<string> k_mers;
	gen(k, k_mers);
	for (auto& k_mer : k_mers) {
		if (distance > d(k_mer, dna)) {
			distance = d(k_mer, dna);
			median = k_mer;
		}
	}
	cout << median;
	return 0;
}
