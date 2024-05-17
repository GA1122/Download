static enum test_return cache_redzone_test(void)
{
#ifndef HAVE_UMEM_H
    cache_t *cache = cache_create("test", sizeof(uint32_t), sizeof(char*),
                                  NULL, NULL);

     
    struct sigaction old_action;
    struct sigaction action = { .sa_handler = SIG_IGN, .sa_flags = 0};
    sigemptyset(&action.sa_mask);
    sigaction(SIGABRT, &action, &old_action);

     
    char *p = cache_alloc(cache);
    char old = *(p - 1);
    *(p - 1) = 0;
    cache_free(cache, p);
    assert(cache_error == -1);
    *(p - 1) = old;

    p[sizeof(uint32_t)] = 0;
    cache_free(cache, p);
    assert(cache_error == 1);

     
    sigaction(SIGABRT, &old_action, NULL);

    cache_destroy(cache);

    return TEST_PASS;
#else
    return TEST_SKIP;
#endif
}