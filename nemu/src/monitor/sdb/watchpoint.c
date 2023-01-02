#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char str[32];
  u_int32_t value;

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp() {
  assert(free_);
  WP* useWP = free_;
  free_ = free_->next;
  useWP->next = head;
  head = useWP;
  return head;
}

void free_wp(u_int32_t idx) {
  WP* dummyhead = NULL;
  dummyhead->next = head;
  while (dummyhead->next) {
    if (dummyhead->next->NO != idx) dummyhead = dummyhead->next;
    else {
      WP* wp = dummyhead->next;
      dummyhead->next = dummyhead->next->next;
      wp->next = free_;
      free_ = wp;
      printf("watchpoint %d:%s is delated.\n", wp->NO, wp->str); 
      break;
    }
  }
}

bool if_change() {
  bool re = false;
  WP* wp = head;
  bool tmp = true;
  bool *success = &tmp;
  while (wp) {
    u_int32_t newvalue = expr(wp->str, success);
    if (newvalue == wp->value) {;}
    else {
			printf("watchpoint %d:%s is changed.\n", wp->NO, wp->str);
			printf("The old value is %d.\n", wp->value);
			printf("The new value is %d.\n", newvalue);
			wp->value = newvalue;
      re = true;
    }
    wp = wp->next;
  }
  return re;
}

void show_watchpoint() {
  WP* wp = head;
  if (!wp) printf("watchpoint is None.\n"); 
  while (wp) {
    printf("watchpoint %d:%s's value is %d.\n", wp->NO, wp->str, wp->value);
    wp = wp->next;
  }
}