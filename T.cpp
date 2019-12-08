#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

std::string BuildStringUsingPrefixFunction(const std::vector<size_t> &prefix) {
  std::string result_str = (prefix.empty() ? "" : "a");
  for (size_t i = 1; i < prefix.size(); ++i) {
    if (prefix[i] == 0) {
      size_t prefix_elem = prefix[i - 1];
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

int main() {
  size_t num;
  std::cin >> num;
  std::vector<size_t> prefix(num);
  for (size_t i = 0; i < num; ++i) {
    std::cin >> prefix[i];
  }
  std::cout << BuildStringUsingPrefixFunction(prefix);

  return 0;
}