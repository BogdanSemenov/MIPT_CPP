#include <iostream>
#include <vector>
#include <algorithm>

unsigned int a, b;
unsigned int cur = 0;
unsigned int nextRand() {
  cur = cur * a + b;
  return cur >> 8u;
}

int main() {
  size_t values_num, quires_num;
  constexpr size_t base_2_power_24 = 1u << 24u;
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cin >> values_num >> quires_num >> a >> b;
  std::vector<unsigned int> values(base_2_power_24, 0);
  std::vector<unsigned int> sum_segment(base_2_power_24, 0);
  unsigned int l, r, add;
  for (size_t i = 0; i < values_num; ++i) {
    add = nextRand();
    l = nextRand();
    r = nextRand();
    if (l > r) {
      std::swap(l, r);
    }
    values[l] += add;
    values[r + 1] -= add;
  }
  sum_segment[0] = values[0];
  unsigned int current_sum = values[0];
  for (size_t i = 1; i < base_2_power_24; ++i) {
    current_sum += values[i];
    sum_segment[i] = current_sum + sum_segment[i - 1];
  }
  unsigned int rsq = 0;
  for (size_t i = 0; i < quires_num; ++i) {
    l = nextRand();
    r = nextRand();
    if (l > r) {
      std::swap(l, r);
    }
    if (l == 0) {
      rsq += sum_segment[r];
    } else {
      rsq += sum_segment[r] - sum_segment[l - 1];
    }
  }
  std::cout << rsq;

  return 0;
}