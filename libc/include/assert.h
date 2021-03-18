#ifndef DAXOS_ASSERT_H
#define DAXOS_ASSERT_H
#include <stdio.h>

#define assert(test) \
  ((test) \
    ? (print_f("assertion failed, " __FILE__ ":" __LINE__ ": " #test), /*panic()*/) \
    : 0)
#endif //DAXOS_ASSERT_H
