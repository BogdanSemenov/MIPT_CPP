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

std::vector<size_t> GetSubstringPosition(const std::string &matched_string, const std::string &pattern) {
  auto prefix = PrefixFunction(matched_string);
  std::vector<size_t> substring_position;
  for (size_t i = 0; i < prefix.size(); ++i) {
    if (prefix[i] == pattern.length()) {
      substring_position.push_back(i - 2 * pattern.length());
    }
  }
  return substring_position;
}

int main() {
  std::string text, pattern;
  std::cin >> text >> pattern;
  std::string matched_string = pattern + '#' + text;
  auto substring_position = GetSubstringPosition(matched_string, pattern);
  for (const auto &pos : substring_position) {
    std::cout << pos << std::endl;
  }

  return 0;
}