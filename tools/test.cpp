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

  if (argc < 2)
    return 1;

  fstream outfile;
  outfile.open(string(argv[1])+".out", ios::out);
  if (!outfile.is_open())
    return 1;

  marisa::Keyset keyset1;
  marisa::Keyset keyset2;
  fstream newfile;
  newfile.open(argv[1], ios::in);
  if (newfile.is_open()) {
     string tp;
     while(getline(newfile, tp)) {
       int len = tp.length();
       const char *str = tp.c_str();
       keyset1.push_back(str, len);
       clen = usx3.compress_simple(str, len, cbuf);
       keyset2.push_back(cbuf, clen);
       tot += len;
       ctot += clen;
       line_count++;
       if (line_count % 10000 == 0)
         printf(".");
       dlen = usx3.decompress_simple(cbuf, clen, dbuf);
       dbuf[dlen] = 0;
       outfile << dbuf << endl;
     }
     newfile.close();
  }
  printf("\nTotal len: %d\n", tot);
  printf("Compressed len: %d\n", ctot);
  printf("No. of lines: %d\n", line_count);

  marisa::Trie trie1;
  trie1.build(keyset1);
  trie1.save("uncompressed.msa");

  marisa::Trie trie2;
  trie2.build(keyset2, MARISA_BINARY_TAIL);
  trie2.save("compressed.msa");

  marisa::Agent agent;
  agent.set_query("i remember when a");
  while (trie1.common_prefix_search(agent)) {
    std::cout.write(agent.key().ptr(), agent.key().length());
    std::cout << ": " << agent.key().id() << std::endl;
  }
  return 0;
}

