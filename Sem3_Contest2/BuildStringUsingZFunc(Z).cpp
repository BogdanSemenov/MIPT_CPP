/*
 Вам задан массив Z-функции некоторой строки. Ваша задача - найти лексикографически минимальную строку,
 массив Z-функции которой совпадет с данным. Алфавит для построения ответа: a-z.
Input format
На входе в первой строке дано число N - размер массива префикс-функции (0 ≤ N ≤ 10^6).
 На второй строке через пробел идут сами элементы. Гарантируется, что исходный массив является
 корректным массивом Z-функции.

Output format
Выведите искомую строку.

Examples
Input	        Output
5               aaaab
5 3 2 1 0
 */

#include <iostream>
#include <vector>
#include <string>

std::string GetStringFromPrefixFunction(const std::vector<int> &prefix) {
  std::string result_str = (prefix.empty() ? "" : "a");
  for (int i = 1; i < prefix.size(); ++i) {
    if (prefix[i] == 0) {
      int prefix_elem = prefix[i - 1];
      char result_letter = 'a';
      while (prefix_elem > 0) {
        result_letter = std::max(result_letter, result_str[prefix_elem]);
        prefix_elem = prefix[prefix_elem - 1];
      }
      result_str += ++result_letter;
    } else {
      result_str += result_str[prefix[i] - 1];
    }
  }
  return result_str;
}

std::vector<int > GetPrefixFromZFunction(const std::vector<int> &z_func) {
  std::vector<int> prefix_func(z_func.size(), 0);
  for(int i = 1; i < prefix_func.size(); ++i) {
    for (int j = z_func[i] - 1; j >= 0; --j) {
      if (prefix_func[i + j] > 0) {
        break;
      } else {
        prefix_func[i + j] = j + 1;
      }
    }
  }
  return prefix_func;
}

std::string GetStringFromZFunction(const std::vector<int> &z_func) {
  auto prefix_func = GetPrefixFromZFunction(z_func);
  return GetStringFromPrefixFunction(prefix_func);
}

int main() {
  int num;
  std::cin >> num;
  std::vector<int> z_func(num);
  for (int i = 0; i < num; ++i) {
    std::cin >> z_func[i];
  }
  std::cout << GetStringFromZFunction(z_func);

  return 0;
}