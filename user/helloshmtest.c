#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int id1 = shmget(42, 1);
  printf("shmget(42, 1) = %d\n", id1);  // باید 0 یا یه عدد >= 0 باشه

  int id2 = shmget(42, 1);
  printf("shmget(42, 1) again = %d\n", id2);  // باید همون id1 باشه

  int id3 = shmget(99, 2);
  printf("shmget(99, 2) = %d\n", id3);  // باید متفاوت باشه

  printf("test done\n");
  exit(0);
}