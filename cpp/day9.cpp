#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>

std::vector<long long> convert_to_nums(std::string input) {
  std::string buff = "";
  std::vector<long long> out;

  for(char c : input) {
    if(c == ' ' && buff.size() != 0) {
      out.push_back(std::stoll(buff));
      buff = "";
    }
    else {
      buff += c;
    }
  }

  if(buff.size() != 0)
    out.push_back(std::stoll(buff));

  return out;
}

std::vector<long long> get_differences(std::vector<long long> input) {
  std::vector<long long> out;
  for(int i=0;i < input.size()-1;i++) {
    out.push_back(input[i+1] - input[i]);
  }

  return out;
}

bool check_all_zeroes(std::vector<long long> input) {
  for(auto num : input) {
    if(num != 0)
      return false;
  }
  return true;
}

long long predict_backwards(std::vector<long long> input) {
  bool zeroes = check_all_zeroes(input);
  if(zeroes)
    return 0;

  std::vector<long long> diffs = get_differences(input);
  return input[0] - predict_backwards(diffs);
}

long long predict_value(std::vector<long long> input) {
  bool zeroes = check_all_zeroes(input);
  if(zeroes)
    return 0;

  std::vector<long long> diffs = get_differences(input);
  return input[input.size() - 1] + predict_value(diffs);
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  std::vector<std::vector<long long>> numbers;
  while(file) {
    getline(file, input);
    if(input.size() == 0)
      continue;

    numbers.push_back(convert_to_nums(input));
  }
  file.close();

  long long sum = 0;
  long long sum2 = 0;
  for(auto list : numbers) {
    long long predicted = predict_value(list);
    long long predicted2 = predict_backwards(list);
    sum += predicted;
    sum2 += predicted2;

    std::cout << "predicted: " << predicted << "\n------\n";
  }

  std::cout << "solution: " << sum << std::endl;
  std::cout << "solution for part2: " << sum2 << std::endl;
  return 0;
}
