#include <iostream>
#include <vector>
#include <queue>


int IncreaseFirstDigit(int number) {
  const size_t thousand = 1000;
  int temp = number;
  if (number / thousand != 9) {
    temp += thousand;
  }
  return temp;
}

int DecreaseLastDigit(int number) {
  const size_t ten = 10;
  int temp = number;
  if (number % ten != 1) {
    --temp;
  }
  return temp;
}

int RightRotation(int number) {
  const size_t ten = 10;
  const size_t thousand = 1000;
  int temp = number / ten + thousand * (number % ten);
  return temp;
}

int LeftRotation(int number) {
  const size_t ten = 10;
  const size_t thousand = 1000;
  int temp = number / thousand + ten * (number % thousand);
  return temp;
}

void AddNumber(int curr, int input, std::vector<bool>& used,
    std::vector<int>& prev, std::queue<int>& q) {
  if (input != curr && !used[input]) {
    used[input] = true;
    q.push(input);
    prev[input] = curr;
  }
}

void BFS(int first_number, int second_number, std::vector<int>& prev) {
  const size_t MAX_SIZE = 10000;
  std::vector<bool> used(MAX_SIZE, false);
  std::queue<int> q;
  q.push(first_number);
  used[first_number] = true;
  int first = 0;
  int second = 0;
  int third = 0;
  int fourth = 0;

  while (!used[second_number]) {
    int curr = q.front();
    q.pop();

    first = IncreaseFirstDigit(curr);
    second = DecreaseLastDigit(curr);
    third = RightRotation(curr);
    fourth = LeftRotation(curr);
    AddNumber(curr, first, used, prev, q);
    AddNumber(curr, second, used, prev, q);
    AddNumber(curr, third, used, prev, q);
    AddNumber(curr, fourth, used, prev, q);
  }
}

std::vector<int> FindPath() {
  const size_t MAX_SIZE = 10000;
  int first_number = 0;
  int second_number = 0;
  std::cin >> first_number >> second_number;

  std::vector<int> prev(MAX_SIZE, -1);

  BFS(first_number, second_number, prev);

  std::vector<int> path;
  while (second_number != -1) {
    path.push_back(second_number);
    second_number = prev[second_number];
  }

  return { path.rbegin(), path.rend() };
}


int main() {
  auto vec = FindPath();

  for (auto i = vec.begin(); i < vec.end(); ++i) {
    std::cout << *i << std::endl;
  }

  return 0;
}
