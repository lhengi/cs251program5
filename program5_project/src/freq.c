#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hmap.h"


/**
 * file:  freq.c
 *
 * desc:  demo program using hmap with integer
 *        values.
 *
 *        Program reads strings from stdin and uses an hmap
 *        to track the frequency (# occurrences) of each distinct 
 *        string.
 *
 *          map:  strings --> #occurrences 
 *
 *                 (KEY)  -->   (VALUE)
 *
 * NOTES:
 *
 *        hmap and target must be compiled accordingly to have int
 *        values.  gcc invoked with -DIVAL flag
 *        See makefile. 
 *
 *        You can run on the given short input file as:
 *
 *            ./freq < data
 *
 */



void analyze() {
  char word[128];
  int max_len = 0;
  int len;
  int i, count;

  HMAP * dict0 = hmap_create(0,1.0);


  while(scanf("%s", word) == 1) {
    len = strlen(word);
    if(len > max_len)
      max_len = len;
    if(hmap_contains(dict0, word)){
      int count = hmap_get(dict0, word);
      hmap_set(dict0, word, count+1);
    }
    else {
      hmap_set(dict0, word, 1);
    }
  }
  KV_PAIR *pairs = hmap_extract_kv_pairs(dict0);

  int n = hmap_size(dict0);

  for(i=0; i<n; i++) {
    printf("%10s    %4i\n", pairs[i].key, pairs[i].val);
  }

  hmap_free(dict0, 0);
}

  
  

  


int main(int argc, char *argv[]) {
  analyze();
}
