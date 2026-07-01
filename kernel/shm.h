#ifndef SHM_H
#define SHM_H

#define SHM_MAX_SEGS   16   // حداکثر تعداد segment های همزمان
#define SHM_MAX_PAGES   4   // حداکثر صفحه در هر segment (= 16KB)

struct shm_segment {
  int   key;                        // شناسه‌ی منطقی که process ها باهاش صداش می‌زنن
  int   ref_count;                  // چند process الان این segment رو map کردن
  int   npages;                     // تعداد صفحات فیزیکی
  char *frames[SHM_MAX_PAGES];     // آدرس فیزیکی هر صفحه (از kalloc)
  int   in_use;                     // آیا این slot در آرایه اشغاله؟
};

void shminit(void);

#endif