#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

std::vector<std::string> matrix;

struct Point {
  int x;
  int y;

  const Point operator+(const Point &other) {
    return { x + other.x, y + other.y };
  }
};

inline bool in_bounds(Point &p) {
  return p.x >= 0 && p.x < matrix[0].size() && p.y >= 0 && p.y < matrix.size();
}

int bfs(Point start, int step_limit) {
  int visited[matrix.size()][matrix[0].size()];
  for(int i=0;i < matrix.size();i++)
    for(int j=0;j < matrix[0].size();j++)
      visited[i][j] = -1;

  Point delta[] = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
  std::queue<std::pair<Point, int>> queue;
  queue.push({ start, 0 });
  int counter = 0; // count only when we reached the limit
  while(!queue.empty()) {
    Point curr = queue.front().first;
    int curr_dist = queue.front().second;
    queue.pop();

    if(visited[curr.y][curr.x] != -1)
      continue;

    visited[curr.y][curr.x] = curr_dist;

    // check every direction
    for(int i=0;i < 4;i++) {
      Point next = curr + delta[i];
      if(!in_bounds(next) || matrix[next.y][next.x] == '#' || curr_dist == step_limit)
        continue;

      queue.push({ curr + delta[i], curr_dist + 1 });
    }
  }

  for(int i=0;i < matrix.size();i++)
    for(int j=0;j < matrix[i].size();j++)
      if(visited[i][j] % 2 == 0)
        counter++;

  return counter;
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

  int start_x, start_y;
  for(int y=0;y < matrix.size();y++) {
    for(int x=0;x < matrix[y].size();x++) {
      if(matrix[y][x] == 'S') {
        start_x = x;
        start_y = y;

        break;
      }
    }
  }

  std::cout << "solution1: " << bfs({ start_x, start_y }, 64) << std::endl;
  return 0;
}
