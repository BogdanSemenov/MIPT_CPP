/*
 Программисту Васе не повезло — вместо отпуска его послали в командировку, на научную конференцию.
 Надо повышать уровень знаний, сказал начальник, важная конференция по криптографии, проводится во
 Франции — а там шифровали еще во времена Ришелье и взламывали чужие шифры еще во времена Виета.
Вася быстро выяснил, что все луврские картины он уже где-то видел, вид эйфелевой башни приелся ему
 еще раньше, чем мышка стерла его с коврика, а такие стеклянные пирамиды у нас делают надо всякими
 киосками и сомнительными забегаловками. Одним словом, смотреть в Париже оказалось просто не на что,
 рыбу половить негде, поэтому Васе пришлось посещать доклады на конференции.
Один из докладчиков, в очередной раз пытаясь разгадать шифры Бэкона, выдвинул гипотезу, что ключ к
 тайнам Бэкона можно подобрать, проанализировав все возможные подстроки произведений Бэкона.
«Но их же слишком много!» — вслух удивился Вася.
«Нет, не так уж и много!» — закричал докладчик — «подсчитайте и вы сами убедитесь!».
Тем же вечером Вася нашел в интернете полное собрание сочинений Бэкона. Он написал программу,
 которая переработала тексты в одну длинную строку, выкинув из текстов все пробелы и знаки препинания.
 И вот теперь Вася весьма озадачен — а как же подсчитать количество различных подстрок этой строки?

Input format
На входе дана непустая строка, полученная Васей. Строка состоит только из строчных латинских символов.
 Ее длина не превосходит 5000 символов.

Output format
Выведите количество различных подстрок этой строки.

Examples
Input	Output
aaba      8
 */

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