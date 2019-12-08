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