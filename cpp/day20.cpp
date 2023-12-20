#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
#include <utility>
#include <vector>
#include <queue>

enum ModuleType {
  BROADCAST,
  FLIP_FLOP,
  CONJUCTION
};

struct Module {
  ModuleType type; // type of the module
  bool state; // current state of the module
};

std::map<std::string, Module> modules;
std::map<std::string, int> cycles;
std::map<std::string, std::vector<std::string>> connections;
int iteration = 1;

long long low_counter = 0;
long long high_counter = 0;

std::vector<std::string> parse_connected(std::string input) {
  std::vector<std::string> out;
  std::string buff = "";

  for(char c : input) {
    if(c == ',') {
      out.push_back(buff);
      buff = "";
    }
    else if(c == ' ') {
      continue;
    }
    else {
      buff += c;
    }
  }

  if(buff != "")
    out.push_back(buff);

  return out;
}

void add_module(std::string name, ModuleType type, std::vector<std::string> connected) {
  Module curr = { .type = type, .state = false };
  modules[name] = curr;
  connections[name] = connected;

  return;
}

void simulate_pulse(bool signal) {
  std::queue<std::pair<std::string, bool>> processing_queue;
  processing_queue.push({ "broadcaster", signal });

  while(!processing_queue.empty()) {
    std::string module_name = processing_queue.front().first;
    bool curr_signal = processing_queue.front().second;
    processing_queue.pop();

    if(curr_signal)
      high_counter++;
    else
      low_counter++;

    if(modules[module_name].type == BROADCAST) {
      for(auto mod : connections[module_name])
        processing_queue.push({ mod, curr_signal });
    }
    else if(modules[module_name].type == FLIP_FLOP) {
      if(curr_signal == false) {
        modules[module_name].state = !modules[module_name].state;
        for(auto mod : connections[module_name]) {
          if(mod == "mg" && modules[module_name].state && cycles[module_name] == -1)
            cycles[module_name] = iteration;
          processing_queue.push({ mod, modules[module_name].state });
        }
      }
    }
    else {
      // check memory
      bool good = true;
      for(auto connection : connections) {
        for(auto connection_mods : connection.second) {
          if(connection_mods == module_name)
            good = good && modules[connection.first].state;
        }
      }
      good = !good;

      modules[module_name].state = good;
      for(auto mod : connections[module_name]) {
        if(mod == "mg" && modules[module_name].state && cycles[module_name] == -1)
          cycles[module_name] = iteration;
        processing_queue.push({ mod, modules[module_name].state });
      }
    }
  }
}

bool check_done() {
  for(auto module : cycles) {
    if(module.second == -1)
      return false;
  }

  return true;
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  while(file) {
    getline(file, input);
    if(input.size() == 0)
      continue;

    std::vector<std::string> connected = parse_connected(input.substr(input.find('>') + 1));
    std::string module_name = input.substr(1, input.find('-') - 2);
    if(input[0] == '%')
      add_module(module_name, FLIP_FLOP, connected);
    else if(input[0] == '&')
      add_module(module_name, CONJUCTION, connected);
    else
      add_module("broadcaster", BROADCAST, connected);
  }
  file.close();

  // part 1
  for(int i=0;i < 1000;i++)
    simulate_pulse(false);
  
  std::cout << "high_counter: " << high_counter << std::endl;
  std::cout << "low_counter: " << low_counter << std::endl;
  std::cout << "solution1: " << high_counter * low_counter << std::endl;

  // reset circuits
  for(auto module : modules)
    modules[module.first].state = false;
  // part 2
  for(auto connection : connections) {
    for(auto connected_module : connection.second)
      if(connected_module == "mg")
        cycles[connection.first] = -1;
  }

  while(true) {
    simulate_pulse(false);

    if(check_done())
      break;

    iteration++;
  }
  long long lcm = 1;
  for(auto module : cycles)
    lcm = std::lcm(lcm, module.second);
  std::cout << "solution2: " << lcm << std::endl;

  return 0;
}
