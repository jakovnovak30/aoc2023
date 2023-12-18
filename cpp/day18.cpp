#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

struct Entry {
  char dir;
  long long len;
};

long long calculate_area(std::vector<Entry> &plan) {
  long long x=0, y=0; // starting point
  std::vector<std::pair<long long, long long>> points;

  long long edge = 0;
  for(Entry &e : plan) {
    switch (e.dir) {
      case 'R':
        x += e.len;
        break;
      case 'L':
        x -= e.len;
        break;
      case 'U':
        y -= e.len;
        break;
      case 'D':
        y += e.len;
        break;
    }

    edge += e.len;

    points.push_back({ x, y });
  }

  long long area = 0;
  for(int i=0;i < points.size();i++) {
    int next_i = (i + 1) % points.size();
    area += (points[i].first * points[next_i].second - points[i].second * points[next_i].first);
  }

  area /= 2;
  return area + edge/2 + 1;
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  std::vector<Entry> plan;
  std::vector<Entry> plan2;
  while(file) {
    getline(file, input);

    if(input.size() == 0)
      continue;

    plan.push_back({ input[0], std::stoi(input.substr(input.find(' ') + 1, input.find('(') - 1))});

    std::string hex = input.substr(input.find('(') + 2, input.size() - input.find('(') - 3);
    char smjer = hex[5];
    switch(smjer) {
      case '0':
      smjer = 'R';
      break;
      case '1':
      smjer = 'D';
      break;
      case '2':
      smjer = 'L';
      break;
      case '3':
      smjer = 'U';
      break;
    }
    std::string::size_type sz = 0;   // alias of size_t
    long long len = std::stoll(hex.substr(0, 5), &sz, 16);

    plan2.push_back( { smjer, len } );
  }
  file.close();

  int area1 = calculate_area(plan);
  long long area2 = calculate_area(plan2);

  std::cout << "solution1: " << area1 << std::endl;
  std::cout << "solution2: " << area2 << std::endl;
  return 0;
}
