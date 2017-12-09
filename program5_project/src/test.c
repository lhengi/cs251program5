#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hmap.h"

unsigned djb(char *str)
{
  unsigned long hash = 5381;
  int c;

  while ((c = *str++) != '\0')
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

unsigned djbXOR(char *str)
{
  unsigned long hash = 5381;
  int c;

  while ((c = *str++) != '\0')
    hash = hash * 33 ^ c;   // xor

  return hash;
}


void test_dict(int display) {
  char word[128];
  int max_len = 0;
  int len;

  HMAP * dict0 = hmap_create(0,1.0);
  HMAP * dict1 = hmap_create(0,1.0);
  HMAP * dict2 = hmap_create(0,1.0);
  HMAP * dict3 = hmap_create(0,1.0);

  hmap_set_hfunc(dict0, NAIVE_HFUNC);
  hmap_set_user_hfunc(dict2, djb, "DJB hash function");
  hmap_set_user_hfunc(dict3, djbXOR, "DJB XOR hash function");

  while(scanf("%s", word) == 1) {
    len = strlen(word);
    if(len > max_len)
      max_len = len;
    hmap_insert(dict0, word);
    hmap_insert(dict1, word);
    hmap_insert(dict2, word);
    hmap_insert(dict3, word);
    if(!hmap_contains(dict0, word))
      fprintf(stderr, "hmap failure (0)\n");
    if(!hmap_contains(dict1, word))
      fprintf(stderr, "hmap failure (1)\n");
  }
  printf("Dictionary Read.  Longest word:  %i letters\n", max_len);
  // sanity check on removal
  hmap_insert(dict0, "wonka");
  if(!hmap_contains(dict0, "wonka"))
    fprintf(stderr, "hmap failure (2)\n");

  hmap_remove(dict0, "wonka");
  if(hmap_contains(dict0, "wonka"))
    fprintf(stderr, "hmap failure (3)\n");

  if(display) {
      printf("----- NAIVE HASH FUNCTION COLLISION PROFILE -----\n");
      hmap_display(dict0);
      printf("----- WEIGHTED HASH FUNCTION COLLISION PROFILE-----\n");
      hmap_display(dict1);
  }

  hmap_print_stats(dict0);
  hmap_print_stats(dict1);
  hmap_print_stats(dict2);
  hmap_print_stats(dict3);

  hmap_free(dict0, 0);
  hmap_free(dict1, 0);
  hmap_free(dict2, 0);
  hmap_free(dict3, 0);
}

  
  

  


int main(int argc, char *argv[]) {
  int display=0;
  int i;
  unsigned coeff;

  for(i=1; i<argc; i++) {
    if(strcmp(argv[i], "-d")==0)
      display = 1;
    else if(strcmp(argv[i], "-c")==0){
      i++;
      if(i==argc){
        printf("usage error with -c flag: no coeff given\n");
        printf("  using default coefficient\n");
      }
      coeff = atoi(argv[i]);
      if(sscanf(argv[i],"%u", &coeff) != 1) {
        printf("usage error with -c flag: invalid coefficient '%s'\n",
            argv[i]);
        printf("  using default coefficient\n");
      }
      else {
        hmap_set_coeff(coeff);
      }
    }
    else {
        printf("usage error: unknown option '%s'\n", argv[i]); 
        return 0;
    }
  }
  test_dict(display);
}
