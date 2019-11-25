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
  std::vector<size_t> log_2_;

 public:
  explicit SparseTable(const std::vector<T> &values) : log_2_(values.size(), 0) {
    const size_t table_size = values.size();
    for (size_t i = 2; i < table_size; ++i) {
      log_2_[i] = log_2_[i / 2] + 1;
    }
    sparse_table_.push_back(values);
    for (size_t delta = 2; delta <= table_size; delta *= 2) {
      std::vector<T> next_level_values(table_size - delta + 1);
      for (size_t i = 0; i < next_level_values.size(); ++i) {
        next_level_values[i] = function_(sparse_table_.back()[i], sparse_table_.back()[i + delta / 2]);
      }
      sparse_table_.push_back(next_level_values);
    }
  }

  T Query(size_t left, size_t right) const {
    size_t log_2 = log_2_[right - left];
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
  size_t value_num, queries_num, first_value;
  std::cin >> value_num >> queries_num >> first_value;
  std::vector<size_t> values(value_num);
  values[0] = first_value;
  for (size_t i = 1; i < value_num; ++i) {
    values[i] = MakeValue<size_t>::MakeNewValue(values[i - 1]);
  }
  SparseTable<size_t, Min<size_t>> sparse_table(values);
  size_t left, right;
  std::cin >> left >> right;
  size_t answer = 0;
  for (size_t i = 1; i < queries_num; ++i) {
    answer = sparse_table.Query(std::min(left - 1, right - 1), std::max(left - 1, right - 1));
    left = (17 * left + 751 + answer + 2 * i) % value_num + 1;
    right = (13 * right + 593 + answer + 5 * i) % value_num + 1;
  }
  answer = sparse_table.Query(std::min(left - 1, right - 1), std::max(left - 1, right - 1));
  std::cout << left << ' ' << right << ' ' << answer;

  return 0;
}
