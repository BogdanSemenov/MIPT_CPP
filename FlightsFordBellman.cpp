/*Между N населенными пунктами совершаются пассажирские рейсы на машинах времени.

В момент времени 0 вы находитесь в пункте A. Вам дано расписание рейсов. Требуется оказаться в пункте B
 как можно раньше (то есть в наименьший возможный момент времени).

При этом разрешается делать пересадки с одного рейса на другой. Если вы прибываете в некоторый пункт в момент
 времени T, то вы можете уехать из него любым рейсом, который отправляется из этого пункта в момент времени
 T или позднее (но не раньше).

Input format

В первой строке вводится число N – количество населенных пунктов (1 ≤ N ≤ 1000). Вторая строка содержит два
 числа A и B – номера начального и конечного пунктов. В третьей строке задается K – количество рейсов
 (0 ≤ K ≤ 1000). Следующие K строк содержат описания рейсов, по одному на строке. Каждое описание представляет
 собой четверку целых чисел. Первое число каждой четверки задает номер пункта отправления, второе – время
 отправления, третье – пункт назначения, четвертое – время прибытия. Номера пунктов – натуральные числа из
 диапазона от 1 до N. Пункт назначения и пункт отправления могут совпадать. Время измеряется в некоторых
 абсолютных единицах и задается целым числом, по модулю не превышающим 10^9. Поскольку рейсы совершаются на
 машинах времени, то время прибытия может быть как больше времени отправления, так и меньше, или равным ему.

Гарантируется, что входные данные таковы, что добраться из пункта A в пункт B всегда можно.

Output format

Выведите минимальное время, когда вы сможете оказаться в пункте B.

 Input      Output
2            0
1 1
2
1 1 2 10
1 10 1 9

1           -10
1 1
3
1 3 1 -5
1 -5 1 -3
1 -4 1 -10

5           -10
1 2
6
1 0 3 10
4 2 2 -10
4 14 2 -7
3 10 2 10
2 0 4 2
3 10 4 12
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

struct Flights {
  size_t departure_place;
  int departure_time;
  size_t arrival_place;
  int arrival_time;

  Flights(size_t departure_place, int departure_time, size_t arrival_place, int arrival_time)
  : departure_place(departure_place),
    departure_time(departure_time),
    arrival_place(arrival_place),
    arrival_time(arrival_time) {}
};

std::vector<int> GetMinTime(std::vector<Flights> &flights, const size_t start) {
  const int INF = std::numeric_limits<int>::max();
  std::vector<int> min_time(flights.size() + 1, INF);
  min_time[start] = 0;
  for (int i = 0; i < flights.size(); ++i) {
    for (auto flight : flights) {
      if (min_time[flight.departure_place] <= flight.departure_time) {
        min_time[flight.arrival_place] = std::min(min_time[flight.arrival_place], flight.arrival_time);
      }
    }
  }
  return min_time;
}

int main() {
  int n, start, finish, m;
  std::cin >> n >> start >> finish >> m;
  std::vector<Flights> flights;

  for (int i = 0; i < m; ++i) {
    size_t departure_place;
    int departure_time;
    size_t arrival_place;
    int arrival_time;
    std::cin >> departure_place >> departure_time >> arrival_place >> arrival_time;
    flights.emplace_back(departure_place, departure_time, arrival_place, arrival_time);
  }

  auto min_time = GetMinTime(flights, start);
  std::cout << min_time[finish];

  return 0;
}
