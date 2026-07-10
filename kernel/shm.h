#ifndef SHM_H
#define SHM_H

#define SHM_MAX_SEGS   16           
#define SHM_MAX_PAGES   4            

struct shm_segment {
  int   key;                     
  int   ref_count;            
  int   npages;                  
  char *frames[SHM_MAX_PAGES];   
  int   in_use;                  
};

void shminit(void);

#endif