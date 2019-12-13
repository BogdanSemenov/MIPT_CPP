#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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

size_t GetSubstringNum(const std::string &str) {
  std::string reverse_string, string;
  size_t substr_count = 0;
  for (const auto &symbol : str) {
    string += symbol;
    reverse_string = { string.rbegin(), string.rend() };
    auto prefix = PrefixFunction(reverse_string);
    size_t max_prefix = *std::max_element(prefix.begin(), prefix.end());
    substr_count += reverse_string.length() - max_prefix;
  }
  return substr_count;
}

int main() {
  std::string str;
  std::cin >> str;
  std::cout << GetSubstringNum(str);

  return 0;
}