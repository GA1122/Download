main(int argc ATTRIBUTE_UNUSED, char **argv ATTRIBUTE_UNUSED) {
    int i, a, ret = 0;
    int subset = 0;

    initializeLibxml2();

    for (a = 1; a < argc;a++) {
        if (!strcmp(argv[a], "-v"))
	    verbose = 1;
        else if (!strcmp(argv[a], "-u"))
	    update_results = 1;
        else if (!strcmp(argv[a], "-quiet"))
	    tests_quiet = 1;
	else {
	    for (i = 0; testDescriptions[i].func != NULL; i++) {
	        if (strstr(testDescriptions[i].desc, argv[a])) {
		    ret += runtest(i);
		    subset++;
		}
	    }
	}
    }
    if (subset == 0) {
	for (i = 0; testDescriptions[i].func != NULL; i++) {
	    ret += runtest(i);
	}
    }
    if ((nb_errors == 0) && (nb_leaks == 0)) {
        ret = 0;
	printf("Total %d tests, no errors\n",
	       nb_tests);
    } else {
        ret = 1;
	printf("Total %d tests, %d errors, %d leaks\n",
	       nb_tests, nb_errors, nb_leaks);
    }
    xmlCleanupParser();
    xmlMemoryDump();

    return(ret);
}
