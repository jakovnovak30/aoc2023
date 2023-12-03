#include <cctype>
#include <cmath>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <map>

struct mapEntry {
  int first;
  int second;
  bool valid;
};

std::vector<std::string> parts;
std::map<std::pair<int, int>, mapEntry *> gears;
int height, width;

char peek(int x, int y, int delta_x, int delta_y) {
  if(x + delta_x <= 0 || x + delta_x >= width)
    return '\0';
  else if(y + delta_y <= 0 || y + delta_y >= height)
    return '\0';

  return parts[y + delta_y][x + delta_x];
}

bool check_suroundings(int i, int j) {
  bool found = false;

  for(int delta_i = -1; delta_i <= 1; delta_i++) {
    for(int delta_j = -1; delta_j <= 1; delta_j++) {
      if(peek(i, j, delta_i, delta_j) != '\0'
          && peek(i, j, delta_i, delta_j) != '.'
          && !std::isdigit(peek(i, j, delta_i, delta_j)))
        found = true;
    }
  }

  return found;
}

bool check_suroundings2(int i, int j, int number) {
  bool found = false;

  for(int delta_i = -1; delta_i <= 1; delta_i++) {
    for(int delta_j = -1; delta_j <= 1; delta_j++) {
      if(peek(i, j, delta_i, delta_j) == '*') {
        found = true;

        int x = i + delta_i, y = j + delta_j;
        std::pair<int, int> kljuc = std::make_pair(x, y);
        if(gears.find(kljuc) == gears.end()) {
          mapEntry *entry = new mapEntry { .first = number, .second = 0, .valid = false };
          gears.insert(std::make_pair(kljuc, entry));
        }
        else {
          if(gears[kljuc]->first == number || gears[kljuc]->second == number) {
            continue;
          }
          else if(gears[kljuc]->second != 0) {
            gears[kljuc]->valid = false;
          }
          else {
            gears[kljuc]->second = number;
            gears[kljuc]->valid = true;
          }
        }
      }
    }
  }

  return found;
}

bool count_number(int y, int x_start, int x_stop, int number) {
  bool count = false;

  for(int i=x_start; i < x_stop;i++) {
    if(check_suroundings2(i, y, number))
      count = true;
  }

  return count;
}

int main() {
  std::fstream file("../src-exe/input.txt");

  std::string input;
  while(file) {
    getline(file, input);
    if(input.size() == 0)
      continue;
    
    parts.push_back(input);
  }
  file.close();

  height = parts.size();
  width = parts[0].size();

  for(int i=0; i < height;i++) {

    int number_start_pos = -1;
    int number_stop_pos = -1;

    for(int j=0; j < width;j++) {
      if(std::isdigit(parts[i][j]) && number_start_pos == -1) {
        number_start_pos = j;
      }
      else if((!std::isdigit(parts[i][j]) || j == width-1) && number_start_pos != -1) {
        number_stop_pos = j;
        int number = std::stoi(parts[i].substr(number_start_pos, number_stop_pos));
        count_number(i, number_start_pos, number_stop_pos, number);

        number_start_pos = -1;
        number_stop_pos = -1;
      }
    }
  }

  int rez = 0;
  for(auto it=gears.begin();it != gears.end();it++) {
    if(it->second->valid)
      rez += it->second->first * it->second->second;
  }

  std::cout << "resenje: " << rez << std::endl;
  return 0;
}
