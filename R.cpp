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

bool IsSubstring(const std::string &pattern, const std::string &text) {
  auto prefix_func = PrefixFunction(pattern + "#" + text);
  for (const auto &prefix : prefix_func) {
    if (prefix == pattern.length()) {
      return true;
    }
  }
  return false;
}

void GetShotInfo(const std::string &first_gun_info, const std::string &second_gun_info) {
  std::string bullet = second_gun_info + "1";
  std::string no_bullet = second_gun_info + "0";
  bool is_substr_with_bullet = IsSubstring(first_gun_info, bullet + bullet);
  bool is_substr_without_bullet = IsSubstring(first_gun_info, no_bullet + no_bullet);
  if(is_substr_with_bullet && is_substr_without_bullet) {
    std::cout << "Random";
  } else if (is_substr_with_bullet) {
    std::cout << "Yes";
  } else {
    std::cout << "No";
  }
}

int main() {
  size_t num;
  std::cin >> num;
  std::string first, second;
  std::cin >> first >> second;
  GetShotInfo(first, second);

  return 0;
}