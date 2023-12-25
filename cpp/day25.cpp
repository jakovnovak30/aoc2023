#include <bits/stdc++.h>

std::map<std::string, std::set<std::string>> edges;
std::vector<std::string> nodes;

void parseNode(std::string node, std::string node_list) {
  if(edges.find(node) == edges.end())
    edges[node] = std::set<std::string>();

  std::string buff = "";
  for(char c : node_list) {
    if(c == ' ') {
      edges[node].insert(buff);
      if(edges.find(buff) == edges.end())
        edges[buff] = std::set<std::string>();
      edges[buff].insert(node);
      buff = "";
    }
    else {
      buff += c;
    }
  }

  if(buff != "") {
    edges[node].insert(buff);
    if(edges.find(buff) == edges.end())
      edges[buff] = std::set<std::string>();
    edges[buff].insert(node);
  }

  return;
}

std::set<std::string> visited;
std::set<std::pair<std::string, std::string>> ignore_set;

std::vector<std::string> bfs(std::string start, std::string end) {
  std::queue<std::pair<std::string, std::vector<std::string>>> queue; // start and path on queue
  std::set<std::string> visited;

  queue.push({ start, std::vector<std::string>() });
  while(!queue.empty()) {
    std::string curr = queue.front().first;
    std::vector<std::string> path = queue.front().second;
    queue.pop();

    if(visited.find(curr) != visited.end())
      continue;
    visited.insert(curr);
    path.push_back(curr);

    if(curr == end)
      return path;

    for(auto connection : edges[curr]) {
      if(ignore_set.find({ curr, connection }) != ignore_set.end()
          || ignore_set.find({ connection, curr }) != ignore_set.end())
        continue;

      queue.push({ connection, path });
    }
  }

  return std::vector<std::string>();
}

int flood_fill(std::string start) {
  std::queue<std::string> queue;
  std::set<std::string> visited;
  
  queue.push(start);
  while(!queue.empty()) {
    std::string curr = queue.front();
    queue.pop();

    if(visited.find(curr) != visited.end())
      continue;
    visited.insert(curr);

    for(auto connection : edges[curr]) {
      if(ignore_set.find({ curr, connection }) != ignore_set.end()
          || ignore_set.find({ connection, curr }) != ignore_set.end())
        continue;

      queue.push(connection);
    }
  }

  return visited.size();
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  while(file) {
    std::getline(file, input);
    if(input.size() == 0)
      continue;

    std::string node = input.substr(0, input.find(':'));
    std::string node_list = input.substr(input.find(':') + 2);
    parseNode(node, node_list);
  }
  file.close();

  // all edge keys are our nodes!
  for(auto edge : edges)
    nodes.push_back(edge.first);

  while(true) {
    int i1 = rand() % nodes.size(), i2 = rand() % nodes.size();
    for(int i=0;i < 3;i++) {
      visited.clear();
      if(i1 == i2)
        continue;
      std::vector<std::string> path = bfs(nodes[i1], nodes[i2]);
      std::string prev = "";
      for(auto node : path) {
        if(prev == "") {
          prev = node;
        }
        else {
          ignore_set.insert({ prev, node });
          prev = node;
        }
      }
    }

    if(bfs(nodes[i1], nodes[i2]).size() == 0)
      break;
    else
      ignore_set.clear();
  }

  const int group1 = flood_fill(nodes[0]);
  const int group2 = nodes.size() - group1;
  std::cout << "group1: " << group1 << " " << "group2: " << group2 << std::endl;
  std::cout << "solution: " << group1 * group2 << std::endl;
  return 0;
}
