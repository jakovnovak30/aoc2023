#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <map>

struct Point {
  int x;
  int y;

  const Point operator+(const Point &other) {
    return { x + other.x, y + other.y };
  }
};

std::vector<std::string> matrix;
std::map<int, std::pair<int, int>> memo;

inline bool in_bounds(Point &p) {
  return p.x >= 0 && p.x < matrix[0].size() && p.y >= 0 && p.y < matrix.size();
}

int bfs(Point start, int step_limit) {
  bool visited[matrix.size()][matrix[0].size()];
  for(int i=0;i < matrix.size();i++)
    for(int j=0;j < matrix[0].size();j++)
      visited[i][j] = false;

  Point delta[] = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
  std::queue<std::pair<Point, int>> queue;
  queue.push({ start, 0 });
  int counter_even = 0; // count only when we reached the limit
  int counter_odd = 0;
  while(!queue.empty()) {
    Point curr = queue.front().first;
    int curr_dist = queue.front().second;
    queue.pop();

    if(visited[curr.y][curr.x] || curr_dist > step_limit)
      continue;

    counter_even += (curr_dist % 2 == 0 && curr_dist);
    counter_odd += (curr_dist % 2 == 1);
    memo[curr_dist] = { counter_even, counter_odd };
    visited[curr.y][curr.x] = curr_dist;

    // check every direction
    for(int i=0;i < 4;i++) {
      Point next = curr + delta[i];
      if(!in_bounds(next) || matrix[next.y][next.x] == '#')
        continue;
      else
        queue.push({ curr + delta[i], curr_dist + 1 });
    }
  }

  return counter_even;
}

long long solution2() {
  long long counter = 0;

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

  // part 2
  // 26501365 = 65 + (202300 * 131)
  //  -> 65 is the number of steps from the center to the edge
  //  -> 131 is the width of one square, so we travel through 202300 squares in each direction!                      
  //  diamond shape -> inside are n^2 odd and (n+1)^2 even counted squares (n = 202300)
  //  corners -> tiles which have an distance > 65 from centre!
  //  final formula : n^2 * even_count + (n + 1)^2 * odd_count + n * even_corner_count - (n + 1) * odd_corner_count
  bfs({ start_x, start_y }, 131);
  const long long n = 202300;
  long long even_count = memo[130].first;
  long long odd_count = memo[130].second;

  long long even_corner_count = memo[130].first - memo[65].first;
  long long odd_corner_count = memo[130].second - memo[65].second;

  long long solution2 = n*n * even_count + (n+1)*(n+1) * odd_count + n * even_corner_count - (n+1) * odd_corner_count;

  std::cout << "solution2: " << solution2 << std::endl;
  return 0;
}
