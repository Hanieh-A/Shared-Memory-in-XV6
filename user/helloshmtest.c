// #include "kernel/types.h"
// #include "user/user.h"

// int
// main(void)
// {
//   int shmid = shmget(42, 1);
//   if(shmid < 0){
//     printf("shmget failed\n");
//     exit(1);
//   }

//   int pid = fork();

//   if(pid < 0){
//     printf("fork failed\n");
//     exit(1);
//   }

//   if(pid == 0){
//     char *addr = (char*)shmat(shmid);
//     pause(10);
//     printf("child read: %s\n", addr);
//     exit(0);
//   } else {
//     char *addr = (char*)shmat(shmid);
//     addr[0] = 'H';
//     addr[1] = 'e';
//     addr[2] = 'l';
//     addr[3] = 'l';
//     addr[4] = 'o';
//     addr[5] = '\0';
//     printf("parent wrote: %s\n", addr);
//     wait(0);
//     printf("sharing test done\n");
//     exit(0);
//   }
// }


#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int shmid = shmget(42, 1);
  if(shmid < 0){ printf("shmget failed\n"); exit(1); }

  int pid = fork();
  if(pid < 0){ printf("fork failed\n"); exit(1); }

  if(pid == 0){
    char *addr = (char*)shmat(shmid);
    pause(10);
    printf("child read: %s\n", addr);
    int r = shmdt(addr);
    printf("child shmdt = %d\n", r);  // باید 0 باشه
    exit(0);
  } else {
    char *addr = (char*)shmat(shmid);
    addr[0]='H'; addr[1]='e'; addr[2]='l';
    addr[3]='l'; addr[4]='o'; addr[5]='\0';
    printf("parent wrote: %s\n", addr);
    int r = shmdt(addr);
    printf("parent shmdt = %d\n", r);  // باید 0 باشه
    wait(0);
    // حالا ref_count=0 شده، segment باید آزاد شده باشه
    // اگه دوباره shmget کنیم باید slot جدید بگیریم
    int shmid2 = shmget(42, 1);
    printf("shmget after free = %d\n", shmid2);  // باید 0 باشه (همون slot)
    printf("all done\n");
    exit(0);
  }
}