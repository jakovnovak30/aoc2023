#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

std::vector<std::string> matrix;
bool visited[300][300];
int max_path = 0;

void dfs(std::pair<int, int> curr, int length) {
  if(visited[curr.second][curr.first])
    return;
  visited[curr.second][curr.first] = true;

  // dosli smo do kraja
  if(curr.second == matrix.size() - 1) {
    if(length > max_path)
      max_path = length;
    visited[curr.second][curr.first] = false;
    return;
  }

  // provjeri sve opcije
  char current_symbol = matrix[curr.second][curr.first];
  static int dx[] = {1, -1, 0, 0};
  static int dy[] = {0, 0, -1, 1};
  static std::map<char, int> directions = { {'>', 0}, {'<', 1}, {'^', 2}, {'v', 3} };
  if(current_symbol == '.') {
    for(int i=0;i < 4;i++) {
      int next_x = curr.first + dx[i];
      int next_y = curr.second + dy[i];
      if(matrix[next_y][next_x] != '#')
        dfs({ next_x, next_y }, length + 1);
    }
  }
  else {
    int next_x = curr.first + dx[directions[current_symbol]];
    int next_y = curr.second + dy[directions[current_symbol]];
    if(matrix[next_y][next_x] != '#')
      dfs({ next_x, next_y }, length + 1);
  }

  visited[curr.second][curr.first] = false;
  return;
}

void dfs2(std::pair<int, int> curr, int length) {
  if(visited[curr.second][curr.first])
    return;
  visited[curr.second][curr.first] = true;

  // dosli smo do kraja
  if(curr.second == matrix.size() - 1) {
    if(length > max_path)
      max_path = length;
  }
  else {
    // provjeri sve opcije
    char current_symbol = matrix[curr.second][curr.first];
    static int dx[] = {1, -1, 0, 0};
    static int dy[] = {0, 0, -1, 1};
    for(int i=0;i < 4;i++) {
      int next_x = curr.first + dx[i];
      int next_y = curr.second + dy[i];
      if(matrix[next_y][next_x] != '#')
        dfs2({ next_x, next_y }, length + 1);
    }
  }

  visited[curr.second][curr.first] = false;
  return;
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  while(file) {
    getline(file, input);
    if(input.size() == 0)
      continue;

    matrix.push_back(input);
  }
  file.close();

  std::pair<int, int> start;
  for(int i=0;i < matrix[0].size();i++) {
    if(matrix[0][i] == '.') {
      start = { i, 0 };
      break;
    }
  }

  // initialize visited
  visited[start.second][start.first] = true;
  dfs2({ start.first, start.second + 1 }, 1);

  std::cout << "max_path: " << max_path << std::endl;
  return 0;
}
