#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

std::map<std::string, std::pair<std::string, std::string>> karta;
std::string instructions;

int get_steps(std::string curr, int inst_index) {
  if(curr[2] == 'Z')
    return 0;

  if(instructions[inst_index] == 'L')
    return 1 + get_steps(karta[curr].first, (inst_index + 1) % instructions.size());
  else
    return 1 + get_steps(karta[curr].second, (inst_index + 1) % instructions.size());
}

int main() {
  std::fstream file("./input.txt");

  std::string input;

  getline(file, instructions);
  std::cout << "instructions.size: " << instructions.size() << "\n";

  while(file) {
    getline(file, input);
    if(input.size() == 0)
      continue;

    std::string current = input.substr(0, input.find_first_of(" "));
    std::string left = input.substr(input.find("(") + 1, input.find(",") - input.find("(") - 1);
    std::string right = input.substr(input.find(", ") + 2, input.find(")") - input.find(", ") - 2);

    karta[current] = std::make_pair(left, right);
  }
  file.close();

  // std::cout << "DEBUG OUTPUT:\n";
  // for(auto it=karta.begin();it != karta.end();it++) {
  //   std::cout << "trenutni: " << it->first << " " << it->second.first << " " << it->second.second << std::endl;
  // }
  
  // PART 1
  // int steps = get_steps("AAA", 0);
  // std::cout << "solution: " << steps << std::endl;

  // PART 2
  std::vector<int> steps;
  for(auto it=karta.begin();it != karta.end();it++) {
    if(it->first[2] == 'A')
      steps.push_back(get_steps(it->first, 0));
  }

  for(int step : steps) {
    std::cout << step << std::endl;
  }

  return 0;
}
