/*
 Examples
Input	                Output
abracadabra      0 0 0 1 0 1 0 1 2 3 4
 */

#include <iostream>
#include <string>
#include <vector>

std::vector<size_t> PrefixFunction(const std::string &str) {
  const size_t str_size = str.length();
  std::vector<size_t> prefix(str_size, 0);
  for (size_t i = 1; i < str_size; ++i) {
    size_t count = prefix[i - 1];
    while (str[i] != str[count] && count > 0) {
      count = prefix[count - 1];
    }
    if (str[i] == str[count]) {
      ++count;
    }
    prefix[i] = count;
  }
  return prefix;
}

int main() {
  std::string str;
  std::cin >> str;
  auto prefix = PrefixFunction(str);
  for (const auto &it : prefix) {
    std::cout << it << ' ';
  }

  return 0;
}