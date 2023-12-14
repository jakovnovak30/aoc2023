#include <iostream>
#include <fstream>
#include <vector>
#include <map>

std::vector<std::string> matrix;

inline bool in_bounds(int x, int y) {
  return x >= 0 && x < matrix[0].size() && y >= 0 && y < matrix.size();
}

void move_rocks(int x, int y, int dx, int dy) {
  int next_y = y - dy;
  int next_x = x - dx;
  while(in_bounds(next_x, next_y) &&
          matrix[next_y][next_x] == '.') {
    next_x -= dx;
    next_y -= dy;
  }

  if(in_bounds(next_x, next_y) && matrix[next_y][next_x] == 'O') {
    matrix[y][x] = 'O';
    matrix[next_y][next_x] = '.';
  }
}
void cycle(){
  for(int i=0;i < matrix.size();i++) {
    for(int j=0;j < matrix[i].size();j++) {
      if(matrix[i][j] == '.')
        move_rocks(j, i, 0, -1);
    }
  }
  for(int i=0;i < matrix.size();i++) {
    for(int j=0;j < matrix[i].size();j++) {
      if(matrix[i][j] == '.')
        move_rocks(j, i, -1, 0);
    }
  }
  for(int i=matrix.size()-1;i >= 0;i--) {
    for(int j=0;j < matrix[i].size();j++) {
      if(matrix[i][j] == '.')
        move_rocks(j, i, 0, 1);
    }
  }
  for(int i=0;i < matrix.size();i++) {
    for(int j=matrix[i].size()-1;j >= 0;j--) {
      if(matrix[i][j] == '.')
        move_rocks(j, i, 1, 0);
    }
  }
}

inline int calculate_sum() {
  int sum = 0;
  for(int i=0;i < matrix.size();i++) {
    for(int j=0;j < matrix[i].size();j++) {
      if(matrix[i][j] == 'O')
        sum += matrix.size() - i;
    }
  }

  return sum;
}

int main() {
  std::fstream file("./input.txt");
  std::string input;
  while(file) {
    getline(file, input);
    if(input.size())
      matrix.push_back(input);
  }
  file.close();

  // part 1
  // for(int i=0;i < matrix.size();i++) {
  //   for(int j=0;j < matrix[i].size();j++) {
  //     if(matrix[i][j] == '.')
  //       move_rocks(j, i, 0, 1);
  //   }
  // }

  // part 2
  for(int i=0;i < 1000;i++) {
    cycle();
  }

  std::cout << "solution: " << calculate_sum() << std::endl;
  return 0;
}
