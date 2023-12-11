#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits.h>
#include <map>

std::vector<std::string> matrix;
std::vector<std::pair<int,int>> galaxies;

std::pair<int, int> minDistance(std::map<std::pair<int,int>, int> dist, std::map<std::pair<int, int>, bool> sptSet) {
  int min_dist = INT_MAX;
  std::pair<int, int> min_index;

  for(int i=0;i < matrix.size();i++) {
    for(int j=0;j < matrix[i].size();j++) {
      std::pair<int, int> curr = {j, i};
      if(sptSet[curr] == false && dist[curr] < min_dist) {
        min_dist = dist[curr];
        min_index = curr;
      }
    }
  }

  if(min_dist == INT_MAX)
    return {-1, -1};

  return min_index;
}

inline bool in_bounds(int x, int y) {
  return x >= 0 && x < matrix[0].size() && y >= 0 && y < matrix.size();
}

inline long long get_len(char x) {
  if(x == 'x')
    return 1000000;

  return 1;
}

// nepotrebno, ali radi!
std::map<std::pair<int,int>, int> dijkstra(int x, int y) {
  std::map<std::pair<int, int>, int> dist;
  std::map<std::pair<int, int>, bool> sptSet;
  int galaxy_counter = galaxies.size() + 1;

  for(int i=0;i < matrix.size();i++) {
    for(int j=0;j < matrix[i].size();j++) {
      dist[{j, i}] = INT_MAX;
      sptSet[{j, i}] = false;
    }
  }
  // initialize distance and visited
  dist[{x, y}] = 0;

  // iterate through every vertex
  while(galaxy_counter) {
    // find current minimum distance for a not processed vertex
    std::pair<int, int> u = minDistance(dist, sptSet);

    if(u.first == -1)
      break;
    // mark the vertex as visited
    sptSet[u] = true;

    // count the galaxy if you've visited it
    if(std::find(galaxies.begin(), galaxies.end(), u) != galaxies.end())
      galaxy_counter--;
      

    // update adjacent vertecies
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    for(int direction = 0;direction < 4;direction++) {
      if(in_bounds(u.first + dx[direction], u.second + dy[direction])) {
        int curr_x = u.first + dx[direction];
        int curr_y = u.second + dy[direction];
        std::pair<int, int> curr = {curr_x, curr_y};

        if(!sptSet[curr] && dist[u] != INT_MAX && dist[u] + get_len(matrix[curr_y][curr_x]) < dist[curr])
          dist[curr] = dist[u] + get_len(matrix[curr_y][curr_x]);
      }
    }
  }

  return dist;
}

long long get_distance(std::pair<int, int> first, std::pair<int, int> second) {
  int xMin = std::min(first.first, second.first), xMax = std::max(first.first, second.first);
  int yMin = std::min(first.second, second.second), yMax = std::max(first.second, second.second);

  long long delta_x = 0, delta_y = 0;
  for(int i=yMin;i < yMax;i++) {
    delta_y += get_len(matrix[i][xMin]);
  }
  for(int j=xMin;j < xMax;j++) {
    delta_x += get_len(matrix[yMax][j]);
  }

  return delta_x + delta_y;
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

  // nadji duplikate
  for(int i=0;i < matrix.size();i++) {
    bool empty = true;
    for(int j=0;j < matrix[i].size();j++) {
      if(matrix[i][j] == '#')
        empty = false;
    }

    if(empty) {
      for(int j=0;j < matrix[i].size();j++)
        matrix[i][j] = 'x';
    }
  }
  for(int j=0;j < matrix[0].size();j++) {
    bool empty = true;
    for(int i=0;i < matrix.size();i++) {
      if(matrix[i][j] == '#')
        empty = false;
    }

    if(empty) {
      for(int i=0;i < matrix.size();i++) {
        matrix[i][j] = 'x';
      }
    }
  }
  
  // find galaxies
  for(int i=0;i < matrix.size();i++) {
    for(int j=0;j < matrix[i].size();j++) {
      if(matrix[i][j] == '#')
        galaxies.push_back({j, i});
    }
  }

  // dijkstra for all galaxies
  long long sum = 0;
  while(!galaxies.empty()) {
    std::pair<int, int> curr = galaxies.back();
    galaxies.pop_back();
    // std::map<std::pair<int,int>, int> distances = dijkstra(curr.first, curr.second);
    // for(auto galaxy : galaxies) {
    //   int dist = distances[galaxy];
    //   std::cout << "debug-distance: " << dist << std::endl;
    //   sum += dist;
    // }

    for(auto galaxy : galaxies) {
      sum += get_distance(curr, galaxy);
    }
  }

  std::cout << sum << std::endl;

  return 0;
}
