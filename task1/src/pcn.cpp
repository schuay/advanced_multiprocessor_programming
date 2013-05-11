#include <stdio.h>

#include "pheet/pheet.h"
typedef pheet::Pheet Pheet;

int
main(int argc,
     char **argv)
{
    {
        Pheet::Environment p;
        Pheet::spawn(printf, " Hello ");
        Pheet::spawn(printf, " World ");
    }
    printf("\n");

    return 0;
}
