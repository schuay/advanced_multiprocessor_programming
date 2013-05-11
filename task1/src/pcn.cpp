#include "pcn.hpp"

#include <pheet/pheet.h>
#include <stdio.h>

typedef pheet::Pheet Pheet;

static void
inc_n_times(PeriodicCountingNetwork<Pheet, int> *pcn,
            const int n)
{
    for (int i = 0; i < n; i++) {
        pcn->incr();
    }
}

int
main(int argc,
     char **argv)
{
    const int n = 50000;

    PeriodicCountingNetwork<Pheet, int> pcn;

    {
        Pheet::Environment p;
        Pheet::spawn(inc_n_times, &pcn, n);
        Pheet::spawn(inc_n_times, &pcn, n);
        Pheet::spawn(inc_n_times, &pcn, n);
        Pheet::spawn(inc_n_times, &pcn, n);
    }

    printf("Expected: %d Actual: %d\n", n * 4, pcn.get_sum());

    return 0;
}
