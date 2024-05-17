testThread(void)
{
    unsigned int i, repeat;
    unsigned int num_threads = sizeof(testfiles) / sizeof(testfiles[0]);
    DWORD results[MAX_ARGC];
    BOOL ret;
    int res = 0;

    xmlInitParser();
    for (repeat = 0; repeat < TEST_REPEAT_COUNT; repeat++) {
        xmlLoadCatalog(catalog);
        nb_tests++;

        for (i = 0; i < num_threads; i++) {
            results[i] = 0;
            tid[i] = (HANDLE) - 1;
        }

        for (i = 0; i < num_threads; i++) {
            DWORD useless;

            tid[i] = CreateThread(NULL, 0,
                                  win32_thread_specific_data,
				  (void *) testfiles[i], 0,
                                  &useless);
            if (tid[i] == NULL) {
                fprintf(stderr, "CreateThread failed\n");
                return(1);
            }
        }

        if (WaitForMultipleObjects(num_threads, tid, TRUE, INFINITE) ==
            WAIT_FAILED) {
            fprintf(stderr, "WaitForMultipleObjects failed\n");
	    return(1);
	}

        for (i = 0; i < num_threads; i++) {
            ret = GetExitCodeThread(tid[i], &results[i]);
            if (ret == 0) {
                fprintf(stderr, "GetExitCodeThread failed\n");
                return(1);
            }
            CloseHandle(tid[i]);
        }

        xmlCatalogCleanup();
        for (i = 0; i < num_threads; i++) {
            if (results[i] != (DWORD) Okay) {
                fprintf(stderr, "Thread %d handling %s failed\n",
		        i, testfiles[i]);
	        res = 1;
	    }
        }
    }

    return (res);
}
