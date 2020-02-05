/*
 Привидение Петя любит играть со своими кубиками. Он любит выкладывать их в ряд и разглядывать
 свое творение. Однако недавно друзья решили подшутить над Петей и поставили в его игровой комнате
 зеркало. Ведь всем известно, что привидения не отражаются в зеркале! А кубики отражаются. Теперь
 Петя видит перед собой N цветных кубиков, но не знает, какие из этих кубиков настоящие, а какие
 — всего лишь отражение в зеркале. Помогите Пете! Выясните, сколько кубиков может быть у Пети.
 Петя видит отражение всех кубиков в зеркале и часть кубиков, которая находится перед ним. Часть
 кубиков может быть позади Пети, их он не видит.

 Input format
Первая строка входного файла содержит число N (1 ≤ N ≤ 100000) и количество различных цветов,
 в которые могут быть раскрашены кубики — M (1 ≤ M ≤ 100000). Следующая строка содержит N целых
 чисел от 1 до M — цвета кубиков.

Output format
Выведите в выходной файл все такие K в порядке возрастания, что у Пети может быть K кубиков

Examples
Input	              Output
6 2                   3 5 6
1 1 2 2 1 1
 */

#include <iostream>
#include <vector>
#include <string>

template<typename T>
std::vector<int> ZFunction(const std::vector<T> &nums) {
  const size_t nums_size = nums.size();
  int left = 0, right = 0;
  std::vector<int> z_func(nums_size, 0);
  z_func[0] = nums_size;
  for (int i = 1; i < nums_size; ++i) {
    z_func[i] = std::max(0, std::min(right - i, z_func[i - left]));
    while (i + z_func[i] < nums_size && nums[z_func[i]] == nums[i + z_func[i]]) {
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
  size_t size, color;
  std::cin >> size >> color;
  std::vector<int> nums(2 * size + 1, -1);
  for (size_t i = 0; i < size; ++i) {
    std::cin >> nums[i];
    nums[2 * size - i] = nums[i];
  }
  auto z_func = ZFunction(nums);

  for (size_t i = size + 1; i < nums.size(); ++i) {
    if (z_func[i] == nums.size() - i && z_func[i] % 2 == 0) {
      std::cout << size - z_func[i] / 2 << ' ';
    }
  }
  std::cout << size;

  return 0;
}