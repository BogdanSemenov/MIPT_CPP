/*
 Дана непустая строка s, длина которой N не превышает 10^6. Будем считать, что элементы
 строки нумеруются от 1 до N.

Для каждой позиции i символа в строке определим Z-блок как наибольшую подстроку, которая
 начинается в этой позиции и совпадает с некоторым началом всей строки s. Значением Z-функции Z(i)
 будем считать длину этого Z-блока. (Заметим, что для первой позиции строки Z-блок совпадает со всей
 строкой, поэтому Z(1) = N. С другой стороны, если s[i] ≠ s[1], то Z(i) = 0).
Z-функция используется в различных алгоритмах обработки строк. В частности, с её помощью можно
 быстро решать задачу о поиске вхождения одной строки в другую («поиск по образцу»).

Требуется для всех i от 1 до N вычислить Z(i).

Input format
Одна строка длины N, 1 ≤ N ≤ 10^6, состоящая из маленьких латинских букв.

Output format
N чисел, разделенные пробелами: Z(1), Z(2), ... Z(N)

Examples
Input	                  Output
abracadabra        11 0 0 1 0 1 0 4 0 0 1
 */

#include <iostream>
#include <vector>
#include <string>

std::vector<int> Z_function(const std::string &str) {
  const size_t str_size = str.length();
  int left = 0, right = 0;
  std::vector<int> z_func(str_size, 0);
  z_func[0] = str_size;
  for (int i = 1; i < str_size; ++i) {
    z_func[i] = std::max(0, std::min(right - i, z_func[i - left]));
  while (i + z_func[i] < str_size && str[z_func[i]] == str[i + z_func[i]]) {
    ++z_func[i];
  }
  if (i + z_func[i] > right) {
    left = i;
    right = i + z_func[i];
  }
  }
  return z_func;
}

int main() {
  std::string str;
  std::cin >> str;
  auto z_func = Z_function(str);
  for (const auto &elem : z_func) {
    std::cout << elem << ' ';
  }

  return 0;
}