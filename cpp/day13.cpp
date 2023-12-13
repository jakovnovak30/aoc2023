#include <iostream>
#include <fstream>
#include <vector>

int part1(std::vector<std::string> matrix) {
  // check vertical
  for(int x=1;x < matrix[0].size();x++) {
    int greske = 0;
    // x oznacava liniju izmedu x-1 i x
    int offset = 0;
    while(true) {
      int x1 = x-1 - offset;
      int x2 = x + offset;

      // bounds check
      if(x1 < 0 || x1 >= matrix[0].size() || x2 < 0 || x2 >= matrix[0].size())
        break;

      for(int i=0;i < matrix.size();i++) {
        if(matrix[i][x1] != matrix[i][x2]) {
          greske++;
        }
      }

      offset++;
    }

    // ako je simetricno, onda vrati rezu
    if(greske == 1)
      return x;
  }

  // check horizontal
  for(int y=1;y < matrix.size();y++) {
    int greske = 0;
    int offset = 0;
    while(true) {
      int y1 = y - 1 - offset;
      int y2 = y + offset;

      // bounds check
      if(y1 < 0 || y1 >= matrix.size() || y2 < 0 || y2 >= matrix.size())
        break;

      for(int i=0;i < matrix[0].size();i++) {
        if(matrix[y1][i] != matrix[y2][i]) {
          greske++;
        }
      }

      offset++;
    }

    // ako je simetricno, onda vrati rezu
    if(greske == 1)
      return y*100;
  }

  // greska
  return -1;
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  std::vector<std::string> matrix;

  int sum = 0;
  while(file) {
    getline(file, input);

    if(input.size() == 0 && matrix.size() != 0) {
      sum += part1(matrix);
      matrix.clear();
    }
    else if(input.size()) {
      matrix.push_back(input);
    }
  }
  file.close();

  std::cout << "solution: " << sum << std::endl;
  return 0;
}
