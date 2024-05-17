testThread(void)
{
    unsigned int i, repeat;
    unsigned int num_threads = sizeof(testfiles) / sizeof(testfiles[0]);
    void *results[MAX_ARGC];
    status_t ret;
    int res = 0;

    xmlInitParser();
    for (repeat = 0; repeat < 500; repeat++) {
        xmlLoadCatalog(catalog);
        for (i = 0; i < num_threads; i++) {
            results[i] = NULL;
            tid[i] = (thread_id) - 1;
        }
        for (i = 0; i < num_threads; i++) {
            tid[i] =
                spawn_thread(thread_specific_data, "xmlTestThread",
                             B_NORMAL_PRIORITY, (void *) testfiles[i]);
            if (tid[i] < B_OK) {
                fprintf(stderr, "beos_thread_create failed\n");
                return (1);
            }
            printf("beos_thread_create %d -> %d\n", i, tid[i]);
        }
        for (i = 0; i < num_threads; i++) {
            ret = wait_for_thread(tid[i], &results[i]);
            printf("beos_thread_wait %d -> %d\n", i, ret);
            if (ret != B_OK) {
                fprintf(stderr, "beos_thread_wait failed\n");
                return (1);
            }
        }

        xmlCatalogCleanup();
        ret = B_OK;
        for (i = 0; i < num_threads; i++)
            if (results[i] != (void *) Okay) {
                printf("Thread %d handling %s failed\n", i, testfiles[i]);
                ret = B_ERROR;
            }
    }
    if (ret != B_OK)
        return(1);
    return (0);
}
