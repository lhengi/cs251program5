#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hmap.h"


/**
 * file:  pval_demo.c
 *
 * desc:  demo program using hmap with void pointer
 *        values (most generic configuration).
 *
 *        simple map created which maps:
 *
 *            names --> linked lists (given by NODE ptrs).
 *
 *            (key)      (value)
 *
 *        program demonstrates:
 *
 *           retrivieval of a list (val)
 *           modification of the list
 *           re-assigning the list as associated value of key.
 *
 * NOTES:
 *
 *        hmap and target must be compiled accordingly to have void * 
 *        values.  gcc invoked with -DPVAL flag
 *        See makefile. 
 *
 *        Also demonstrates how such lists (values) must be deallocated by
 *        the client (e.g., this program) since the hmap is just 
 *        treating them as void pointers.
 *
 */


typedef struct node{
  int val;
  struct node *next;
}NODE;


void print_lst(NODE * lst) {
  if(lst==NULL)
    return;
  printf(" %i \n", lst->val);
  print_lst(lst->next);
}

void free_lst(NODE * lst) {
  if(lst==NULL)
    return;
  free_lst(lst->next);
  free_lst(lst->next);
}

NODE *prepend(NODE *lst, int val) {
  NODE *p = malloc(sizeof(NODE));

  p->val = val;
  p->next = lst;
  return p;
}

int main(int argc, char *argv[]) {
  HMAP * map = hmap_create(0,1.0);
  NODE *lst1, *lst2, *lst3;
  NODE *lst;

  lst1 = prepend(NULL, 99);
  lst2 = prepend(NULL, 888);
  lst3 = prepend(NULL, 123);

  hmap_set(map, "Bob", lst1);
  hmap_set(map, "Mary", lst2);
  hmap_set(map, "Sunil", lst3);


  // retrieve list (value) for Sunil
  lst = hmap_get(map, "Sunil");
  printf("List retrieved for Sunil:\n");
  print_lst(lst);

  // modify list
  lst = prepend(lst, 1024);

  // now put it back
  hmap_set(map, "Sunil", lst);


  // now retrieve it again
  lst = hmap_get(map, "Sunil");
  printf("List retrieved for Sunil AFTER MOD:\n");
  print_lst(lst);


  // before freeing map, we have to retrieve and free all of the
  //   values (lists) since the hmap has no idea how to free them.

  NODE ** vals = (NODE **)hmap_extract_values(map);

  int n = hmap_size(map);
  int i;

  for(i=0; i<n; i++) 
    free_lst(vals[i]);
  free(vals);

  // now we can free the map itself
  hmap_free(map,0);

}
