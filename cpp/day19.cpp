#include <iostream>
#include <fstream>
#include <vector>
#include <map>

struct Rule {
  bool is_final;
  std::string next_label;

  // if is_final == false
  char attribute;
  char comparator;
  int constant;
};
std::map<std::string, std::vector<Rule>> rules;

std::vector<Rule> parse_Rules(std::string rule_str) {
  std::vector<Rule> out;
  std::string buff = "";
  for(char c : rule_str) {
    if(c == ',') {
      char attribute = buff[0];
      char comparator = buff[1];
      int constant = std::stoi(buff.substr(2, buff.find(':') - 1));
      std::string next_label = buff.substr(buff.find(':') + 1);

      out.push_back({ .is_final = false, .next_label = next_label ,
                      .attribute = attribute, .comparator = comparator, .constant = constant });

      buff = "";
    }
    else {
      buff += c;
    }
  }

  // final rule is just the next label
  out.push_back({ .is_final = true, .next_label = buff });
  return out;
}

std::map<char, int> parseParts(std::string input) {
  std::map<char, int> out;
  std::string buff = "";
  for(int i=0;i < input.size();i++) {
    if(input[i] == ',' || i == input.size() - 1) {
      if(i == input.size() - 1)
        buff += input[i];
      char attribute = buff[0];
      int value = std::stoi(buff.substr(2));

      out[attribute] = value;
      buff = "";
    }
    else {
      buff += input[i];
    }
  }

  return out;
}

bool compare(int first, char comparator, int second) {
  if(comparator == '<')
    return first < second;
  else
    return first > second;
}

bool check_rules(std::string workflow, std::map<char, int> &input) {
  std::string next_label;
  for(Rule &r : rules[workflow]) {
    if(r.is_final || compare(input[r.attribute], r.comparator, r.constant)) {
      next_label = r.next_label;
      break;
    }
  }

  if(next_label == "A")
    return true;
  else if(next_label == "R")
    return false;

  return check_rules(next_label, input);
}

long long count_combinations(std::map<char, std::pair<short, short>> &attrs) {
  long long mult = 1;
  for(auto range : attrs) {
    if(range.first != 0)
      mult = mult * (range.second.second + 1 - range.second.first);
  }
  return mult;
}

long long check_posibilities(std::string workflow, std::map<char, std::pair<short, short>> attrs) {
  if(workflow == "R")
    return 0;
  else if(workflow == "A") {
    return count_combinations(attrs);
  }

  long long count = 0;
  for(Rule &r : rules[workflow]) {
    std::pair<short, short> curr_range = attrs[r.attribute];

    if(r.is_final) {
      count += check_posibilities(r.next_label, attrs);
    }
    else if(r.comparator == '<') {
      if(attrs[r.attribute].second < r.constant) {
        count += check_posibilities(r.next_label, attrs);
      }
      else if(attrs[r.attribute].first >= r.constant)
        continue;
      else {
        std::pair<short, short> range1 = { attrs[r.attribute].first, r.constant - 1} ;
        std::pair<short, short> range2 = { r.constant, attrs[r.attribute].second };

        attrs[r.attribute] = range1; // call recursion with smaller range
        count += check_posibilities(r.next_label, attrs);
        attrs[r.attribute] = range2; // save the other range for the else case
      }
    }
    else {
      if(attrs[r.attribute].first > r.constant) {
        count += check_posibilities(r.next_label, attrs);
      }
      else if(attrs[r.attribute].second <= r.constant)
        continue;
      else {
        std::pair<short, short> range1 = { r.constant + 1, attrs[r.attribute].second };
        std::pair<short, short> range2 = { attrs[r.attribute].first, r.constant };

        attrs[r.attribute] = range1; // call recursion with bigger range
        count += check_posibilities(r.next_label, attrs);
        attrs[r.attribute] = range2; // check rest with smaller range
      }
    }
  }
  return count;
}

int main() {
  std::fstream file("./input.txt");

  std::string input;
  bool rules_input = true;
  int sum = 0;
  while(file) {
    getline(file, input);

    if(input.size() == 0) {
      rules_input = false;
      continue;
    }

    if(rules_input) {
      std::string key = input.substr(0, input.find('{'));
      std::string rule_str = input.substr(input.find('{') + 1, input.find('}') - input.find('{') - 1);

      // parse the rules for the current label
      std::vector<Rule> rule_list = parse_Rules(rule_str);

      // save the rules for the current label
      rules[key] = rule_list;
    }
    else {
      // parse the input attributes
      std::map<char, int> curr_attributes = parseParts(input.substr(1, input.size() - 2));
      if(check_rules("in", curr_attributes)) {
        for(auto attr : curr_attributes)
          sum += attr.second;
      }
    }
  }
  file.close();

  // part 2
  std::map<char, std::pair<short, short>> attrs;
  attrs['x'] = {1, 4000};
  attrs['m'] = {1, 4000};
  attrs['a'] = {1, 4000};
  attrs['s'] = {1, 4000};
  long long count = check_posibilities("in", attrs);

  std::cout << "solution1: " << sum << std::endl;
  std::cout << "solution2: " << count << std::endl;
  return 0;
}
