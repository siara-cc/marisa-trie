#include <stdio.h>

int main() {

  FILE *fp;

  fp = fopen("word_freq_250k.msa", "r");
  while (!feof(fp)) {
    int c = fgetc(fp);
    if (c == -1)
      break;
    printf("\\x%02x", c);
  }
  fclose(fp);

}

