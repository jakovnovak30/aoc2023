#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

struct Entry {
  unsigned long long src;
  unsigned long long dest;
  unsigned long long offset;
};

struct SeedRange {
  unsigned long long min;
  unsigned long long max;
};

SeedRange *transform_range(SeedRange *range, Entry transformer) {

  if(range->min > transformer.src + transformer.offset || range->max < transformer.src) {
    return nullptr;
  }
  else if(range->min <= transformer.src && range->max >= transformer.src + transformer.offset) {
    return new SeedRange { .min = transformer.dest, .max = transformer.dest + transformer.offset };
  }
  else if(range->min <= transformer.src && range->max < transformer.src + transformer.offset) {
    unsigned long long max_offset = transformer.src + transformer.offset - range->max;

    return new SeedRange { .min = transformer.dest, .max = transformer.dest + max_offset };
  }
  else if(range->min > transformer.src && range->max >= transformer.src + transformer.offset) {
    unsigned long long min_offset = range->min - transformer.src;

    return new SeedRange { .min = transformer.dest + min_offset, .max = transformer.dest + transformer.offset };
  }
  else if(range->min > transformer.src && range->max < transformer.src + transformer.offset) {
    unsigned long long min_offset = range->min - transformer.src;
    unsigned long long max_offset = transformer.src + transformer.offset - range->max;

    return new SeedRange { .min = transformer.dest + min_offset, .max = transformer.dest + max_offset };
  }

  return nullptr;
}

std::vector<unsigned long long> convertToNumbers(std::string input) {
  std::string buff = "";
  std::vector<unsigned long long> out;

  for(char c : input) {
    if(buff != "" && c == ' ') {
      out.push_back(std::stoll(buff));
      buff = "";
    }
    else if(std::isdigit(c)) {
      buff += c;
    }
  }
  // don't forget about the last one
  if(buff != "")
    out.push_back(std::stoll(buff));

  return out;
}

Entry parse_to_entry(std::vector<unsigned long long> numbers) {
  return Entry { .src = numbers[1], .dest = numbers[0], .offset = numbers[2] - 1 };
}

unsigned long long transform_value(long long value, std::vector<Entry> table) {
  for(Entry entry : table) {
    if(value >= entry.src && value <= entry.src + entry.offset) {
      return value - entry.src + entry.dest;
    }
  }

  return value;
}

unsigned long long find_final(long long start, std::vector<std::vector<Entry>> tables) {
  long long curr = start;

  for(std::vector<Entry> table : tables) {
    curr = transform_value(curr, table);
  }

  return curr;
}

std::vector<unsigned long long> transform_value(long long value, long long range, std::vector<Entry> table) {
  std::vector<unsigned long long> results;
  for(Entry entry : table) {
    for(int delta = 0;delta < range;delta++) {
      if(value + delta >= entry.src && value + delta <= entry.src + entry.offset) {
        results.push_back(value - entry.src + entry.dest);
        break;
      }
    }
  }

  return results;
}

unsigned long long find_final(unsigned long long start, unsigned long long range, std::vector<std::vector<Entry>> tables) {
  std::vector<unsigned long long> results;
  results.push_back(start);

  for(std::vector<Entry> table : tables) {
    std::vector<unsigned long long> new_results;
    for(long long result : results) {
      std::vector<unsigned long long> temp = transform_value(result, range, table);


      for(long long num : temp)
        new_results.push_back(num);
    }

    results = new_results;
  }

  unsigned long long min = 1000000000000;
  for(long long num : results) {
    if(num > min)
      min = num;
  }

  return min;
}

int main() {
  std::fstream file("./input.txt");

  std::string input;

  getline(file, input);
  std::string nums = input.substr(input.find(": ") + 2);
  std::vector<unsigned long long> seeds = convertToNumbers(nums); // PARSE OUR SEEDS
  getline(file, input); // get an empty line

  std::vector<std::vector<Entry>> tables;
  std::vector<Entry> curr_table;
  while(file) {
    getline(file, input);

    if(input.size() == 0) {
      if(curr_table.size() != 0) {
        tables.push_back(curr_table);
        curr_table.clear();
      }
    }
    else {
      // ignoriraj naslov tablice...
      if(input.find(":") != input.npos)
        continue;
      else
        curr_table.push_back(parse_to_entry(convertToNumbers(input)));
    }
  }
  file.close();

  // part 1
  unsigned long long min = 0;
  // for(long long seed : seeds) {
  //   long long found = find_final(seed, tables);
  //
  //   std::cout << "found: " << found << "\n";
  //
  //   if(min > found)
  //     min = found;
  // }

  // part 2

  std::vector<SeedRange *> ranges;

  for(int i=0;i < seeds.size();i += 2) {
    unsigned long long seed = seeds[i];
    unsigned long long range = seeds[i+1];
  
    SeedRange *seed_range = new SeedRange { .min = seed, .max = seed + range - 1 };

    ranges.push_back(seed_range);
  }

  for(std::vector<Entry> table : tables) {
    std::vector<SeedRange *> new_ranges; // update the list of ranges after we're done with this table!
    // iterate through every range - table combination
    for(SeedRange *range : ranges) {
      bool atLestOneTransform = false;

      for(Entry entry : table) {
        SeedRange *result = transform_range(range, entry);
        if(result != nullptr && result->min < min) {
          min = result->min;
          std::cout << "debug-result: " << result->min << std::endl;
        }

        if(result != nullptr) {
          std::cout << "debug - pushing range: " << result->min << " " << result->max << "\n";
          new_ranges.push_back(result);
          atLestOneTransform = true;
        }
      }

      if(!atLestOneTransform)
        new_ranges.push_back(range);
    }

    ranges = new_ranges;
  }
  std::cout << "solution: " << min << std::endl;
  return 0;
}
