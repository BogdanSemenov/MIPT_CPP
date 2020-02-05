/*
 Третий день не встает Иван с печи. Все ему надоело: и балалайка, и водка. "Так больше нельзя!"
 - сказал себе Иван, взял револьвер, надел валенки и ушел, хлопнув дверью.
"Здравствуй, Ваня!" - сказал ручной медведь Миша, увидев перед собой хмурого Ивана, стоящего
 на пороге его берлоги - "С чем пожаловал?". "Я хочу сыграть с тобой в одну игру" - сказал Иван,
 достав револьвер из-за пазухи. Миша молча сел за стол. Иван приблизился к столу - "Вращай!" - сказал
 он и дал пистолет в лапы медведю.
Медведь увидел перед собой револьвер неизвестной модели с вращающимся барабаном.
 В некоторые пазы были вставлены пули. Все пазы видны, кроме одного - основного, из которого
 в ствол подается пуля. Медведь крутанул барабан.
- Что дальше? - спросил Миша.
- Стреляй. Либо в себя, либо в меня.
Медведь был очень предан своему хозяину, поэтому не мог его убить. Медведь снова глянул на барабан
 и захотел вычислить, будет ли сейчас произведен выстрел. Ведь от этого зависит его дальнейшая судьба...

Input format
Первая строка содержит целое число n (2 ≤ n ≤ 2*106) - количество пазов в барабане револьвера.
Во второй строке задана строка a, описывающая состояние барабана револьвера до вращения. Строка состоит из n - 1
 символов, где ai = 0, если в i-м пазе по часовой стрелке, начиная от дула, нет пули, и ai = 1, если есть.
В третьей строке задана строка b, описывающая состояние барабана револьвера после вращения в таком же формате.
Гарантируется, что входные данные непротиворечивы.

Output format
Выведите Yes, если выстрел точно будет произведен, No, если выстрела точно не будет или Random,
 если нельзя точно установить, будет ли выстрел.

Examples
Input	 Output
6          Yes
11111
01111

5          No
1100
0110

5          Random
1001
1001

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