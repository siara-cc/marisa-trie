// sample.cc
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "marisa.h"
#include "unishox3.h"

using namespace std;

int main(int argc, char *argv[]) {

  unishox3 usx3;
  char cbuf[150];
  int clen;
  char dbuf[150];
  int dlen;
  int line_count;
  int ctot = 0;
  int tot = 0;

  marisa::Trie trie1;
  trie1.load("uncompressed.msa");

  marisa::Trie trie2;
  trie2.load("compressed.msa");

  marisa::Agent agent;
  agent.set_query(argv[1]);
  trie1.lookup(agent);
  cout << agent.key().id() << endl;

  return 0;
}

