#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
    int r = helloshm();
    printf("returned: %d\n", r);
    exit(0);
}