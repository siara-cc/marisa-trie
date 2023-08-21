#include <fstream>
#include <iostream>

#include "marisa.h"

using namespace std;

double time_taken_in_secs(clock_t t) {
  t = clock() - t;
  return ((double)t)/CLOCKS_PER_SEC;
}

clock_t print_time_taken(clock_t t, const char *msg) {
  double time_taken = time_taken_in_secs(t); // in seconds
  std::cout << msg << time_taken << std::endl;
  return clock();
}

int main(int argc, char *argv[]) {

  marisa::Keyset keyset;

  clock_t t = clock();
  fstream infile;
  infile.open(argv[1], ios::in);
  int line_count = 0;
  if (infile.is_open()) {
    string line;
    string prev_line = "";
    while (getline(infile, line)) {
      if (line == prev_line)
         continue;
      keyset.push_back(line.c_str(), line.length());
      prev_line = line;
      line_count++;
      if ((line_count % 100000) == 0) {
        cout << ".";
        cout.flush();
      }
    }
  }
  infile.close();

  marisa::Trie trie;
  trie.build(keyset, MARISA_LABEL_ORDER);
  printf("\nBuild Keys per sec: %lf\n", line_count / time_taken_in_secs(t) / 1000);
  t = print_time_taken(t, "Time taken for build: ");

  marisa::Agent agent;
  for (std::size_t i = 0; i < keyset.size(); ++i) {
    agent.set_query(keyset[i].ptr(), keyset[i].length());
    if (!trie.lookup(agent) || (agent.key().id() != keyset[i].id())) {
      std::cerr << "error: lookup() failed" << std::endl;
      return 1;
    }
  }
  printf("\nKeys per sec: %lf\n", keyset.size() / time_taken_in_secs(t) / 1000);
  t = print_time_taken(t, "Time taken for retrieve: ");

  return 0;

}
