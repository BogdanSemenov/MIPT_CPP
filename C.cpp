#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

template <typename T>
T gcd(const T &lhs, const T &rhs){
  if (rhs == 0) {
    return lhs;
  }
  return gcd(rhs, lhs % rhs);
}

template<typename T>
struct GCD {
  T operator()(const T &lhs, const T &rhs) const {
    return gcd(lhs, rhs);
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

int main() {
  size_t size;
  std::cin >> size;
  std::vector<size_t> values(size);
  for (size_t i = 0; i < size; ++i) {
    std::cin >> values[i];
  }
  SparseTable<size_t, GCD<size_t>> sparse_table(values);
  size_t queries_num;
  std::cin >> queries_num;
  for (size_t i = 0; i < queries_num; ++i) {
    size_t left, right;
    std::cin >> left >> right;
    auto query = sparse_table.Query(left - 1, right - 1);
    std::cout << query << std::endl;
  }

  return 0;
}