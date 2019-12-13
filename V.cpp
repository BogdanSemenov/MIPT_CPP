#include <iostream>
#include <vector>
#include <string>

std::vector<int> ZFunction(const std::string &str) {
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

std::vector<int> AFunction(const std::string &string) {
  std::vector<int> a_func;
  std::string reversed_string = {string.rbegin(), string.rend()};
  std::string string_concatenation = string + "#" + reversed_string;
  auto z_func = ZFunction(string_concatenation);
  const size_t str_size = string.length();
  for (size_t i = 2 * str_size; i > str_size; --i) {
    a_func.push_back(z_func[i]);
  }
  return a_func;
}

int main() {
  size_t num;
  std::string string;
  std::cin >> num >> string;
  auto a_func = AFunction(string);
  for (const auto &elem : a_func) {
    std::cout << elem << ' ';
  }

  return 0;
}