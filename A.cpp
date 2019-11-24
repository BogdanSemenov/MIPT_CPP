#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

template<typename T>
struct Min {
  T operator()(const T &lhs, const T &rhs) const {
    return std::min(lhs, rhs);
  }
};

template<typename T, typename Function>
class SparseTable {
 private:
  std::vector<std::vector<T>> sparse_table_;
  Function function_;
  std::vector<int> log_2_;

 public:
  explicit SparseTable(const std::vector<T> &values) : log_2_(values.size() + 1, 0) {
    const int table_size = values.size();
    for (int i = 2; i <= table_size; ++i) {
      log_2_[i] = log_2_[i / 2] + 1;
    }
    sparse_table_.resize(log_2_[table_size] + 1, std::vector<T>(table_size + 1));
    for (int i = 0; i <= log_2_[table_size]; ++i) {
      for (int j = 0; j < table_size; ++j) {
        if (i == 0) {
          sparse_table_[i][j] = values[j];
        } else {
          sparse_table_[i][j] = function_(sparse_table_[i - 1][j], sparse_table_[i - 1][j + (1u << (i - 1))]);
        }
      }
    }
  }

  T Query(int left, int right) const {
    if (left > right) {
      std::swap(left, right);
    }
    int log_2 = log_2_[right - left];
    return function_(sparse_table_[log_2][left], sparse_table_[log_2][right - (1u << log_2) + 1]);
  }
};

template<typename T>
struct MakeValue {
  static T MakeNewValue(const T &prev_value) {
    return (23 * prev_value + 21563) % 16714589;
  }
};

int main() {
  int value_num, queries_num;
  int first_value;
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cin >> value_num >> queries_num >> first_value;
  std::vector<int> values(value_num);
  values[0] = first_value;
  for (int i = 1; i < value_num; ++i) {
    values[i] = MakeValue<int>::MakeNewValue(values[i - 1]);
  }
  SparseTable<int, Min<int>> sparse_table(values);
  int left, right;
  std::cin >> left >> right;
  int ans = 0;
  for (int i = 1; i < queries_num; ++i) {
    ans = sparse_table.Query(left - 1, right - 1);
    left = (17 * left + 751 + ans + 2 * i) % value_num + 1;
    right = (13 * right + 593 + ans + 5 * i) % value_num + 1;
  }
  std::cout << left << ' ' << right << ' ' << sparse_table.Query(left - 1, right - 1);

  return 0;
}