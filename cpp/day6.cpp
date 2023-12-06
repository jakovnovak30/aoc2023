#include <cctype>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <assert.h>

std::vector<int> convert_to_numbers(std::string input) {
  std::vector<int> out;
  input = input.substr(input.find(": ") + 2);
  std::string buff = "";

  for(char c : input) {
    if(c == ' ' && buff != "") {
      int temp = std::stoi(buff);
      out.push_back(temp);
      buff = "";
    }
    else if(isdigit(c)) {
      buff += c;
    }
  }

  // check for last
  if(buff != "")
    out.push_back(std::stoi(buff));

  return out;
}

int get_combinations(int time, int record) {
  int counter = 0;

  // speed == i, remaining_time == time - i
  for(int i=1;i < time;i++) {
    counter += i * (time - i) > record;
  }
  return counter;
}

int main() {
  // std::fstream file("./example.txt");
  std::fstream file("./input.txt");

  std::string input;
  unsigned long long time;
  unsigned long long record;
  // get time
  getline(file, input);
  std::vector<int> times = convert_to_numbers(input);
  // get time for part 2
  std::string buff = "";
  for(char c : input) {
    if(std::isdigit(c))
      buff += c;
  }
  time = std::stoull(buff);
  // get records
  getline(file, input);
  std::vector<int> records = convert_to_numbers(input);
  buff = "";
  for(char c : input) {
    if(std::isdigit(c))
      buff += c;
  }
  record = std::stoull(buff);
  assert(times.size() == records.size() && "Error parsing input!");

  // record < speed * remaining_time => valid combination
  int out = 1;
  for(int i=0;i < times.size();i++) {
    int time = times[i];
    int record = records[i];

    int comb = get_combinations(time, record);
    out *= comb;
  }

  unsigned long long counter = 0;
  for(unsigned long long curr_time = 1; curr_time < time; curr_time++) {
    counter += curr_time * (time - curr_time) > record;
  }

  std::cout << "solution1: " << out << std::endl;
  std::cout << "solution2: " << counter << std::endl;
  return 0;
}
