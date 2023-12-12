#include <cctype>
#include <iostream>
#include <fstream>
#include <math.h>
#include <utility>
#include <vector>
#include <map>

// kljuc -> ostali dijelovi, popis ostalih brojeva grupa, akumulator
// vrijednost -> kombinacije
std::map<std::tuple<std::string, std::vector<int>, int>, long long> memo;

std::vector<int> to_nums(std::string input) {
  std::vector<int> out;

  std::string buff = "";
  for(char c : input) {
    if(isdigit(c)) {
      buff.push_back(c);
    }
    else if(c == ',') {
      out.push_back(std::stoi(buff));
      buff = "";
    }
  }

  if(buff.size())
    out.push_back(std::stoi(buff));
  
  return out;
}

bool matching_nums(std::string parts, std::vector<int> numbers) {
  int num_index = 0; // index u polju brojeva
  int temp = 0; // broj pokvarenih u trenutnoj grupi

  for(char part : parts) {
    if(part == '#') {
      temp++;
    }
    else if(part == '.' && temp != 0) {
      if(num_index >= numbers.size() || numbers[num_index] != temp)
        return false;

      num_index++;
      temp = 0;
    }
  }

  // zadnji dio
  if(temp != 0){
    if(num_index >= numbers.size() || numbers[num_index] != temp)
      return false;
    num_index++;
  }
  
  return num_index == numbers.size();
}

int get_arrangements(std::string parts, std::vector<int> numbers) {
  std::vector<int> upitnik_pos; // cuva pozicije svih upitnika
  for(int i=0;i < parts.size();i++) {
    if(parts[i] == '?')
      upitnik_pos.push_back(i);
  }

  std::cout << "debug: " << upitnik_pos.size() << std::endl;
  return 0;
  int counter = 0;
  // idi po svim kombinacijama
  // i -> pr: 00101010101, gdje je broj binarnih znamenki == upitnik_pos.size()
  for(int i=0;i < pow(2, upitnik_pos.size());i++) {
    std::string temp_parts = parts;
    for(int j=0;j < upitnik_pos.size();j++) {
      temp_parts[upitnik_pos[j]] = (i >> j) & 1 ? '.' : '#';
    }

    if(matching_nums(temp_parts, numbers))
      counter++;
  }

  return counter;
}

long long get_arrangements2(std::string parts, std::vector<int> numbers, int acc) {
  std::tuple<std::string, std::vector<int>, int> stanje = { parts, numbers, acc };
  if(memo.find(stanje) != memo.end())
    return memo[stanje];

  int curr_num;
  if(numbers.size()) {
    curr_num = *numbers.begin();
  }
  else {
    curr_num = 0;
  }

  // na kraju smo
  if(parts.size() == 0) {
    if(acc == 0 && numbers.size() == 0 ||
        acc == curr_num && numbers.size() == 1)
      memo[stanje] = 1;
    else
      memo[stanje] = 0;
  }
  // imamo upitnik
  else if(parts[0] == '?') {
    memo[stanje] = get_arrangements2('.' + parts.substr(1), numbers, acc)
                 + get_arrangements2('#' + parts.substr(1), numbers, acc);
  }
  // imamo tocku
  else if(parts[0] == '.') {
    if(acc == 0) {
      memo[stanje] = get_arrangements2(parts.substr(1), numbers, 0);
    }
    else if(acc == curr_num) {
      numbers.erase(numbers.begin());
      memo[stanje] = get_arrangements2(parts.substr(1), numbers, 0);
    }
    else {
      memo[stanje] = 0;
    }
  }
  // imamo hashtag
  else if(parts[0] == '#') {
    if(acc + 1 > curr_num) {
      memo[stanje] = 0;
    }
    else {
      memo[stanje] = get_arrangements2(parts.substr(1), numbers, acc + 1);
    }
  }

  return memo[stanje];
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  int sum = 0;
  long long sum2 = 0;
  while(file) {
    getline(file, input);
    if(input.size() == 0)
      continue;

    std::string parts = input.substr(0, input.find(' '));
    std::string number_string = input.substr(input.find(' ') + 1);
    std::vector<int> numbers = to_nums(number_string);

    // part 1
    // sum += get_arrangements(parts, numbers);

    // part 2
    std::string parts2 = parts;
    std::vector<int> numbers2 = numbers;
    for(int i=0;i < 4;i++) {
      parts2 = parts2 + "?" + parts;
      for(int num : numbers)
        numbers2.push_back(num);
    }

    std::cout << "solution for: " << input << " is: " << get_arrangements2(parts2, numbers2, 0) << std::endl;
    sum2 += get_arrangements2(parts2, numbers2, 0);
    memo.clear();
  }
  file.close();

  std::cout << "solution: " << sum2 << std::endl;
  return 0;
}
