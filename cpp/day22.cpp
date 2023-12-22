#include <algorithm>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <set>
#include <map>

struct Cube {
  int x1, y1, z1;
  int x2, y2, z2;

};

const bool operator<(const Cube &first, const Cube &second) {
  return first.z1 < second.z1 || first.z2 < second.z2;
}

std::map<int, std::vector<int>> supporting_cubes;
std::map<int, std::vector<int>> supported_cubes;

Cube parse_cube(std::string point1, std::string point2) {
  int x1 = std::stoi(point1.substr(0, point1.find_first_of(',')));
  int y1 = std::stoi(point1.substr(point1.find_first_of(',') + 1, point1.find_last_of(',') - point1.find_first_of(',')));
  int z1 = std::stoi(point1.substr(point1.find_last_of(',') + 1));

  int x2 = std::stoi(point2.substr(0, point2.find_first_of(',')));
  int y2 = std::stoi(point2.substr(point2.find_first_of(',') + 1, point2.find_last_of(',') - point2.find_first_of(',')));
  int z2 = std::stoi(point2.substr(point2.find_last_of(',') + 1));

  return { x1, y1, z1, x2, y2, z2 };
}

inline bool is_intersecting(std::vector<Cube> &cubes, int i, int j) {
  if(cubes[j].y2 < cubes[i].y1 || cubes[j].y1 > cubes[i].y2)
    return false;
  if(cubes[j].x2 < cubes[i].x1 || cubes[j].x1 > cubes[i].x2)
    return false;

  return true;
}

bool is_done_falling(std::vector<Cube> &cubes, int curr_index) {
  if(cubes[curr_index].z1 == 0)
    return true;

  for(int i=0;i < curr_index;i++) {
    if(cubes[i].z2 == cubes[curr_index].z1 - 1 && is_intersecting(cubes, i, curr_index))
      return true;
  }

  return false;
}

int simulate_falling(std::vector<Cube> &cubes) {
  int counter = 0;
  for(int i=0;i < cubes.size();i++) {
    if(!is_done_falling(cubes, i))
      counter++;
    while(!is_done_falling(cubes, i)) {
      cubes[i].z1--;
      cubes[i].z2--;
    }
  }

  return counter;
}

void count_supporting(std::vector<Cube> &cubes) {
  for(int i=0;i < cubes.size();i++) {
    // check if the i-th cube is supporting any cubes above...
    for(int j=i+1;j < cubes.size();j++) {
      if(cubes[i].z2 + 1 != cubes[j].z1)
        continue;
      
      if(is_intersecting(cubes, i, j)) {
        supporting_cubes[i].push_back(j); // i is supporting j
        supported_cubes[j].push_back(i); // j is being supported by i // j is being supported by i
      }
    }
  }
}

std::vector<Cube> reset_cube_set(std::vector<Cube> &cubes, int index) {
  std::vector<Cube> cube_vector;
  for(int i=0;i < cubes.size();i++) {
    if(i != index)
      cube_vector.push_back(cubes[i]);
  }

  return cube_vector;
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  std::vector<Cube> cubes;
  while(file) {
    getline(file, input);
    if(input.size() == 0)
      continue;

    std::string point1 = input.substr(0, input.find('~'));
    std::string point2 = input.substr(input.find('~') + 1);

    Cube curr = parse_cube(point1, point2);
    cubes.push_back(curr);
  }
  file.close();

  std::sort(cubes.begin(), cubes.end());
  
  for(int i=0;i < cubes.size();i++) {
    supporting_cubes[i] = std::vector<int>();
    supported_cubes[i] = std::vector<int>();
  }
  simulate_falling(cubes);
  count_supporting(cubes);

  // part 1
  int counter = 0;
  for(auto par : supporting_cubes) {
    bool dobro = true;
    for(int i=0;i < par.second.size();i++) {
      if(supported_cubes[par.second[i]].size() == 1)
        dobro = false;
    }
    counter += dobro;
  }
  std::cout << "solution1: " << counter << std::endl;

  // part 2
  int sum = 0;
  for(int i=0;i < cubes.size();i++) {
    std::vector<Cube> cube_vector = reset_cube_set(cubes, i);
    sum += simulate_falling(cube_vector);
  }
  std::cout << "solution2: " << sum << std::endl;
  return 0;
}
