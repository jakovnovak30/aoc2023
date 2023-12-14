#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#define N_ITER 1000000000

std::vector<std::string> matrix;
std::map<std::vector<std::string>, int> memo;
std::vector<std::vector<std::string>> indices;

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
  int cycle_start = 0;
  int i;
  for(i=0;i < N_ITER;i++) {
    memo[matrix] = calculate_sum();
    indices.push_back(matrix);

    cycle();
    if(memo.find(matrix) != memo.end()) {
      for(int k=0;k < indices.size();k++) {
        if(indices[k] == matrix){
          cycle_start = k;
          break;
        }
      }
      break;
    }
  }
  int map_index = (N_ITER - i - 1) % (i - cycle_start + 1);
  auto key = indices[cycle_start + map_index];

  std::cout << "solution: " << memo[key] << std::endl;
  return 0;
}
