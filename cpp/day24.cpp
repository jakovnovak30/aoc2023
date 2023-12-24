#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

struct Line {
  long long x, y, z;
  long long vel_x, vel_y, vel_z;
};
std::vector<Line> lines;

Line parse_input(std::string input) {
  std::string positions = input.substr(0, input.find('@') - 1);
  std::string velocities = input.substr(input.find('@') + 1); 

  long long x = std::stoll(positions.substr(0, positions.find_first_of(',')));
  long long vel_x = std::stoll(velocities.substr(0, velocities.find_first_of(',')));

  long long y = std::stoll(positions.substr(positions.find_first_of(',') + 1, positions.find_last_of(',') - positions.find_first_of(',') - 1));
  long long vel_y = std::stoll(velocities.substr(velocities.find_first_of(',') + 1, velocities.find_last_of(',') - velocities.find_first_of(',') - 1));

  long long z = std::stoll(positions.substr(positions.find_last_of(',') + 1));
  long long vel_z = std::stoll(velocities.substr(velocities.find_last_of(',') + 1));

  return { x, y, z, 
          vel_x, vel_y, vel_z };
}

// px1 + k1 * vx1 = px2 + k2 * vx2
// py1 + k1 * vy1 = py2 + k2 * vy2
// if the sides are between 200000000000000 and 400000000000000, return true
bool colliding(int i, int j) {
  // if(lines[j].vel_y * lines[i].vel_x - lines[i].vel_y * lines[j].vel_x < 0.00001)
  //   return false;

  double k = double(lines[i].y * lines[i].vel_x - lines[i].vel_y * lines[i].x + lines[j].x * lines[i].vel_y - lines[j].y * lines[i].vel_x)
                  / (lines[j].vel_y * lines[i].vel_x - lines[i].vel_y * lines[j].vel_x);
  double k1 = double(lines[j].y + k * lines[j].vel_y - lines[i].y) / lines[i].vel_y;

  if(k <= 0 || k1 <= 0)
    return false;

  if(k * lines[j].vel_x + lines[j].x >= 200000000000000 && k * lines[j].vel_x + lines[j].x <= 400000000000000
      && k * lines[j].vel_y + lines[j].y >= 200000000000000 && k * lines[j].vel_y + lines[j].y <= 400000000000000) {
    return true;
  }

  return false;
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  while(file) {
    getline(file, input);
    if(input.size() == 0)
      continue;

    lines.push_back(parse_input(input));
  }
  file.close();

  // part 1
  int counter = 0;
  for(int i=0;i < lines.size();i++) {
    for(int j=i + 1;j < lines.size();j++) {
      if(colliding(i, j)) {
        counter++;
      }
    }
  }
  std::cout << "solution1: " << counter << std::endl;

  return 0;
}
