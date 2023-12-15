#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>
#include <string>

using namespace std;

vector<string> split_input(string input) {
  string buff = "";
  vector<string> out;

  for(char c : input) {
    if(c == ',') {
      if(buff.size())
        out.push_back(buff);
      buff = "";
    }
    else {
      buff += c;
    }
  }

  if(buff.size())
    out.push_back(buff);

  return out;
}

int get_hash(string input) {
  int hash = 0;
  for(char c : input) {
    hash += (int) c;
    hash *= 17;
    hash %= 256;
  }

  return hash;
}

vector<int> solution1(vector<string> input) {
  vector<int> out;
  for(auto word : input) {
    out.push_back(get_hash(word));
  }

  return out;
}

int solution2(vector<string> input) {
  vector<pair<string, int>> hash_table[256];
  for(string operation : input) {
    if(operation.find('=') != operation.npos) {
      string label = operation.substr(0, operation.find('='));
      int hash = get_hash(label);
      int number = stoi(operation.substr(operation.find('=') + 1));

      bool found = false;
      for(int i=0;i < hash_table[hash].size();i++) {
        if(hash_table[hash][i].first == label) {
          hash_table[hash][i].second = number;
          found = true;
          break;
        }
      }

      if(!found)
        hash_table[hash].push_back({ label, number });
    }
    else {
      string label = operation.substr(0, operation.find('-'));
      int hash = get_hash(label);

      for(auto it = hash_table[hash].begin();it != hash_table[hash].end();it++) {
        if(it->first == label) {
          hash_table[hash].erase(it);
          break;
        }
      }
    }
  }

  int out = 0;
  // zbroji rezultate
  for(int i=0;i < 256;i++)
    for(int j=0;j < hash_table[i].size();j++)
      out += hash_table[i][j].second * (j + 1) * (i + 1);
  return out;
}

int main() {
  fstream file("./input.txt");

  string input;
  while(file) {
    getline(file, input);

    if(input.size() == 0)
      continue;

    // part 1
    vector<int> sizes = solution1(split_input(input));
    int sum = accumulate(sizes.begin(), sizes.end(), 0);
    cout << "solution1: " << sum << endl;
    // part 2
    int sum2 = solution2(split_input(input));
    cout << "solution2: " << sum2 << endl;
  }
  file.close();

  return 0;
}
