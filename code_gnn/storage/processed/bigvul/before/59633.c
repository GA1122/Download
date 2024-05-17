testThread(void)
{
    unsigned int i, repeat;
    unsigned int num_threads = sizeof(testfiles) / sizeof(testfiles[0]);
    void *results[MAX_ARGC];
    int ret;
    int res = 0;

    xmlInitParser();

    for (repeat = 0; repeat < 500; repeat++) {
        xmlLoadCatalog(catalog);
        nb_tests++;

        for (i = 0; i < num_threads; i++) {
            results[i] = NULL;
            tid[i] = (pthread_t) - 1;
        }

        for (i = 0; i < num_threads; i++) {
            ret = pthread_create(&tid[i], 0, thread_specific_data,
                                 (void *) testfiles[i]);
            if (ret != 0) {
                fprintf(stderr, "pthread_create failed\n");
                return (1);
            }
        }
        for (i = 0; i < num_threads; i++) {
            ret = pthread_join(tid[i], &results[i]);
            if (ret != 0) {
                fprintf(stderr, "pthread_join failed\n");
                return (1);
            }
        }

        xmlCatalogCleanup();
        for (i = 0; i < num_threads; i++)
            if (results[i] != (void *) Okay) {
                fprintf(stderr, "Thread %d handling %s failed\n",
                        i, testfiles[i]);
                res = 1;
            }
    }
    return (res);
}
