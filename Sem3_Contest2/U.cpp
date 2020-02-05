/*
 Ваша цель - передать сверхсекретное сообщение, состоящее из n слов. Так как сообщение секретное,
 вам необходимо его зашифровать так, чтобы в итоге все предложение состояло из одного слова. А вот
 и сам алгоритм:
При склеивании двух слов в одно, удаляется самый длинный префикс второго слова, который совпадает
 с суффиксом первого слова. Например, слова «крошка» и «картошка» склеиваются в «крошкартошка».
Сообщение склеивается слева направо (т.е. сначала склеиваются первые два слова, потом результат с третьим, и так далее).
 Напишите программу, которая зашифрует заданное сообщение

Input format
Первая строка содержит одно целое число n (1 ≤ n ≤ 10^5) — количество слов в сообщении.
Вторая строка содержит n слов, разделенных единичными пробелами. Каждое слово непусто и состоит
 из строчных латинских букв. Суммарная длина слов не превосходит 10^6.

Output format
Выведите в выходной файл зашифрованное сообщение.

Examples
Input	                         Output
3                              bananaba
banana nana nanaba

4                              papamamapapa
papa pama mama mapapa
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>

std::vector<size_t> PrefixFunction(const std::string &string) {
  const size_t str_size = string.length();
  std::vector<size_t> prefix(str_size, 0);
  for (size_t i = 1; i < str_size; ++i) {
    size_t count = prefix[i - 1];
    while (string[i] != string[count] && count > 0) {
      count = prefix[count - 1];
    }
    if (string[i] == string[count]) {
      ++count;
    }
    prefix[i] = count;
  }
  return prefix;
}

void MakeCode(std::string &lhs, std::string &rhs) {
  const size_t substr_size = std::max(0, static_cast<int>(lhs.size() - rhs.size()));
  auto prefix = std::move(PrefixFunction(rhs + "#" + lhs.substr(substr_size)));
  lhs += rhs.substr(prefix.back());
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t num;
  std::cin >> num;
  std::string answer;
  std::cin >> answer;
  for (size_t i = 1; i < num; ++i) {
    std::string string;
    std::cin >> string;
    MakeCode(answer, string);
  }
  std::cout << answer;

  return 0;
}