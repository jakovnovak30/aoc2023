#include <cstdint>
#include <ios>
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <queue>
#include <vector>

using namespace std;

vector<vector<int>> matrix;

enum Direction {
  LEFT = 0,
  RIGHT,
  UP,
  DOWN
};

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

inline bool is_opposite(Direction dir1, Direction dir2) {
  return dir1 == LEFT && dir2 == RIGHT || dir1 == RIGHT && dir2 == LEFT
      || dir1 == UP && dir2 == DOWN || dir1 == DOWN && dir2 == UP;
}

struct State {
  int x, y;
  Direction last_dir;
  int dir_count;
};

const bool operator<(const pair<int, State> first, const pair<int, State> second) {
  return first.first < second.first;
}

int distances[200][200][4][4]; // za svaku tocku, smjer i broj smjera

void dijkstra() {
  // initialize
  priority_queue<pair<int, State>, vector<pair<int, State>>, greater<pair<int, State>>> pq;
  State start1 = { .x = 0, .y = 0, .last_dir = DOWN, .dir_count = 0 };
  State start2 = { .x = 0, .y = 0, .last_dir = RIGHT, .dir_count = 0 };
  pq.push(make_pair(0, start1));
  pq.push(make_pair(0, start2));
  distances[0][0][3][0] = 0;
  distances[0][0][1][0] = 0;

  while(!pq.empty()) {
    int dist = pq.top().first;
    State closest = pq.top().second;
    pq.pop();

    for(int i=LEFT;i <= DOWN;i++) {
      int new_x = closest.x + dx[i], new_y = closest.y + dy[i];

      if(new_x < 0 || new_x >= matrix[0].size() || new_y < 0 || new_y >= matrix.size())
        continue;
      if(is_opposite(closest.last_dir, (Direction) i))
        continue;
      if(closest.last_dir == (Direction) i && closest.dir_count >= 3)
        continue;

      State new_state;
      if(closest.last_dir == (Direction) i)
        new_state = { .x = new_x, .y = new_y, .last_dir = (Direction) i, .dir_count = closest.dir_count + 1};
      else
        new_state = { .x = new_x, .y = new_y, .last_dir = (Direction) i, .dir_count = 1 };
      // provjera udaljenosti
      if(dist + matrix[new_y][new_x] < distances[new_x][new_y][new_state.last_dir][new_state.dir_count]) {

        distances[new_x][new_y][new_state.last_dir][new_state.dir_count] = dist + matrix[new_y][new_x];
        pq.push(make_pair(distances[new_x][new_y][new_state.last_dir][new_state.dir_count], new_state));
      }
    }
  }
}

int main() {
  fstream file("./input.txt");

  string input;
  while(file) {
    getline(file, input);

    if(input.size() == 0)
      continue;

    std::vector<int> numbers;
    for(char c : input) {
      numbers.push_back(c - '0');
    }
    matrix.push_back(numbers);
  }
  file.close();

  // initialize distance arrays
  for(int x=0;x < 200;x++) {
    for(int y=0;y < 200;y++) {
      for(int i=0;i < 4;i++) {
        for(int j=0;j < 4;j++) {
          distances[x][y][i][j] = INT32_MAX;
        }
      }
    }
  }
  dijkstra();

  // part 1
  int minimum = INT32_MAX;
  int x = matrix[0].size()-1, y = matrix.size()-1;
  for(int i=0;i < 4;i++) {
    for(int j=0;j < 4;j++) {
      if(distances[x][y][i][j] < minimum)
        minimum = distances[x][y][i][j];
    }
  }

  cout << "solution: " << minimum << endl;

  // 1010 --> too high

  return 0;
}
