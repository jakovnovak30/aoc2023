#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

int card_to_num(char card) {
    switch(card) {
      case 'A':
        return 12;
        break;
      case 'K':
        return 11;
        break;
      case 'Q':
        return 10;
        break;
      case 'J':
        return 0;
        break;
      case 'T':
        return 9;
        break;
      default:
        return card - '0' - 1; // od 2 do 9 -> tj. od 1 do 8
        break;
    }
}

int get_score(std::string cards) {
  int card_counter[13];
  for(int i=0;i < 13;i++)
    card_counter[i] = 0;

  for(char card : cards) {
    card_counter[card_to_num(card)]++;
  }

  int joker_num = card_counter[0];
  card_counter[0] = 0;
  std::sort(card_counter, card_counter + 13);

  // five of a kind, four of a kind
  card_counter[12] += joker_num;
  if(card_counter[12] == 5 || card_counter[12] == 4)
    return card_counter[12] + 2; // rank 7 i rank 6
  else if(card_counter[12] == 3 && card_counter[11] == 2)
    return 5; // full house
  else if(card_counter[12] == 3)
    return 4; // three of a kind
  else if(card_counter[12] == 2 && card_counter[11] == 2)
    return 3; // two pair
  else if(card_counter[12] == 2)
    return 2; // one pair

  return 1;
}

bool get_bigger_by_max(std::string cards1, std::string cards2) {

  for(int i=0;i < 5;i++) {
    int num1 = card_to_num(cards1[i]);
    int num2 = card_to_num(cards2[i]);
    if(num1 == num2)
      continue;
    else if(num1 < num2)
      return true;
    else
     return false;
  }

  return false;
}

struct Hand {
  std::string cards;
  int bid;

  const bool operator<(const Hand &other) {
    if(get_score(this->cards) < get_score(other.cards)) {
      return true;
    }
    else if(get_score(this->cards) == get_score(other.cards)) {
      // provjeri najvecu
      return get_bigger_by_max(this->cards, other.cards);
    }
    else {
      return false;
    }
  }
};

int main () {
  std::fstream file("./input.txt");

  std::string input;
  std::vector<Hand> hands;
  while(file) {
    getline(file, input);
    if(input.size() == 0)
      continue;

    std::string cards = input.substr(0, input.find(" "));
    int bid = std::stoi(input.substr(input.find(" ") + 1));

    hands.push_back({ .cards = cards, .bid = bid });
  }
  file.close();

  std::sort(hands.begin(), hands.end());
  int score = 0;

  for(int i=0;i < hands.size();i++) {
    score += (i+1) * hands[i].bid;
  }

  std::cout << "solution: " << score << std::endl;
  return 0;
}
