// sample.cc
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "marisa.h"
#include "unishox3.h"

using namespace std;

int main() {

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
  agent.set_query(6190);
  trie2.reverse_lookup(agent);
  dlen = usx3.decompress_simple(agent.key().ptr(), agent.key().length(), dbuf);
  dbuf[dlen] = 0;
  cout.write(dbuf, dlen);
  cout << ": " << agent.key().id() << endl;
  agent.set_query(920924);
  trie2.reverse_lookup(agent);
  dlen = usx3.decompress_simple(agent.key().ptr(), agent.key().length(), dbuf);
  dbuf[dlen] = 0;
  cout.write(dbuf, dlen);
  cout << ": " << agent.key().id() << endl;

  return 0;
}

