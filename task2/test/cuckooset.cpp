#include "cuckooset.hpp"

#include <check.h>

#define ITERS (123456)

START_TEST(test_sanity)
{
    CuckooSet<pheet::Pheet, unsigned long> set;
    fail_unless(1 == 1);
}
END_TEST

START_TEST(empty_is_empty)
{
    CuckooSet<pheet::Pheet, unsigned long> set;
    fail_unless(set.is_empty());
}
END_TEST

START_TEST(empty_is_size_0)
{
    CuckooSet<pheet::Pheet, unsigned long> set;
    fail_unless(set.size() == 0);
}
END_TEST

START_TEST(contains_1)
{
    CuckooSet<pheet::Pheet, unsigned long> set;
    fail_unless(!set.contains(1));
}
END_TEST

START_TEST(contains_2)
{
    CuckooSet<pheet::Pheet, unsigned long> set;
    set.put(1);
    fail_unless(set.contains(1));
}
END_TEST

START_TEST(contains_3)
{
    CuckooSet<pheet::Pheet, unsigned long> set;
    set.put(1);
    set.remove(1);
    fail_unless(!set.contains(1));
}
END_TEST

START_TEST(contains_4)
{
    CuckooSet<pheet::Pheet, unsigned long> set;
    set.put(1);
    fail_unless(set.contains(1));
    fail_unless(!set.contains(2));
}
END_TEST

START_TEST(put_1)
{
    CuckooSet<pheet::Pheet, unsigned long> set;
    set.put(1);
    set.put(1);
    set.put(1);
    fail_unless(set.contains(1));
}
END_TEST

START_TEST(put_2)
{
    CuckooSet<pheet::Pheet, unsigned long> set;

    srand(42);

    unsigned int size = 0;
    for (int i = 0; i < ITERS; i++) {
        const int v = rand();
        if (!set.contains(v)) {
            size++;
        }
        set.put(v);
    }
    fail_unless(set.size() == size);
}
END_TEST

START_TEST(remove_1)
{
    CuckooSet<pheet::Pheet, unsigned long> set;

    srand(42);

    unsigned int size = 0;
    for (int i = 0; i < ITERS; i++) {
        const int v = rand();
        if (!set.contains(v)) {
            size++;
        }
        set.put(v);
    }
    fail_unless(set.size() == size);

    srand(42);

    for (int i = 0; i < ITERS; i++) {
        set.remove(rand());
    }
    fail_unless(set.size() == 0);
}
END_TEST

static void
put_wrapper(CuckooSet<pheet::Pheet, unsigned long> *set,
            const unsigned long v)
{
    set->put(v);
}

static void
contains_wrapper(CuckooSet<pheet::Pheet, unsigned long> *set,
            const unsigned long v)
{
    set->contains(v);
}

static void
remove_wrapper(CuckooSet<pheet::Pheet, unsigned long> *set,
            const unsigned long v)
{
    set->remove(v);
}

START_TEST(smoke_1)
{
    CuckooSet<pheet::Pheet, unsigned long> set;

    {
        pheet::Pheet::Environment env;

        for (int i = 0; i < ITERS; i++) {
            pheet::Pheet::spawn(put_wrapper, &set, i);
        }
    }

    fail_unless(set.size() == ITERS);
}
END_TEST

START_TEST(smoke_2)
{
    CuckooSet<pheet::Pheet, unsigned long> set;

    {
        pheet::Pheet::Environment env;

        for (int i = 0; i < ITERS; i++) {
            pheet::Pheet::spawn(put_wrapper, &set, i);
            pheet::Pheet::spawn(contains_wrapper, &set, (i + 12345) % ITERS);
        }
    }

    fail_unless(set.size() == ITERS);
}
END_TEST

START_TEST(smoke_3)
{
    CuckooSet<pheet::Pheet, unsigned long> set;

    {
        pheet::Pheet::Environment env;

        for (int i = 0; i < ITERS; i++) {
            pheet::Pheet::spawn(put_wrapper, &set, i);
        }
    }

    {
        pheet::Pheet::Environment env;

        for (int i = 0; i < ITERS; i++) {
            pheet::Pheet::spawn(remove_wrapper, &set, i);
        }
    }

    fail_unless(set.size() == 0);
}
END_TEST

static Suite *
create_suite(void)
{
    Suite *s = suite_create(__FILE__);
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_sanity);
    tcase_add_test(tc_core, empty_is_empty);
    tcase_add_test(tc_core, empty_is_size_0);
    tcase_add_test(tc_core, contains_1);
    tcase_add_test(tc_core, contains_2);
    tcase_add_test(tc_core, contains_3);
    tcase_add_test(tc_core, contains_4);
    tcase_add_test(tc_core, put_1);
    tcase_add_test(tc_core, put_2);
    tcase_add_test(tc_core, remove_1);

    TCase *tc_smoke = tcase_create("parallel_smoke");

    tcase_add_test(tc_core, smoke_1);
    tcase_add_test(tc_core, smoke_2);
    tcase_add_test(tc_core, smoke_3);

    suite_add_tcase(s, tc_core);
    suite_add_tcase(s, tc_smoke);

    return s;
}

int
main(int argc __attribute__ ((unused)),
     char **argv __attribute__ ((unused)))
{
    int number_failed;
    Suite *s = create_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
