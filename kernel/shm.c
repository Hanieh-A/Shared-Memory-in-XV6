#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "defs.h"
#include "shm.h"

static struct shm_segment shm_table[SHM_MAX_SEGS];

static struct spinlock shm_lock;

void
shminit(void)
{
    initlock(&shm_lock, "shm");
    for(int i = 0; i < SHM_MAX_SEGS; i++) {
        shm_table[i].in_use = 0;
        shm_table[i].key = 0;
        shm_table[i].ref_count = 0;
        shm_table[i].npages = 0;
        for(int j = 0; j < SHM_MAX_PAGES; j++)
            shm_table[i].frames[j] = 0;
    }
    printf("shminit: shared memory subsystem initialized\n");
}

// key: شناسه‌ی منطقی، npages: تعداد صفحه (1 تا SHM_MAX_PAGES)
// برمی‌گردونه: shmid (index) یا -1 در صورت خطا
int
shmget(int key, int npages)
{
  if(key <= 0 || npages <= 0 || npages > SHM_MAX_PAGES)
    return -1;

  acquire(&shm_lock);

  for(int i = 0; i < SHM_MAX_SEGS; i++){
    if(shm_table[i].in_use && shm_table[i].key == key){
      release(&shm_lock);
      return i;  // همون shmid رو برگردون
    }
  }

  int slot = -1;
  for(int i = 0; i < SHM_MAX_SEGS; i++){
    if(!shm_table[i].in_use){
      slot = i;
      break;
    }
  }
  if(slot == -1){
    release(&shm_lock);
    return -1; 
  }

  for(int j = 0; j < npages; j++){
    char *mem = kalloc();
    if(mem == 0){
      for(int k = 0; k < j; k++){
        kfree(shm_table[slot].frames[k]);
        shm_table[slot].frames[k] = 0;
      }
      release(&shm_lock);
      return -1;
    }
    memset(mem, 0, PGSIZE);
    shm_table[slot].frames[j] = mem;
  }

  shm_table[slot].key       = key;
  shm_table[slot].npages    = npages;
  shm_table[slot].ref_count = 0;
  shm_table[slot].in_use    = 1;

  release(&shm_lock);
  return slot;
}