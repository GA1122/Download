static pid_t start_server(in_port_t *port_out, bool daemon, int timeout) {
    char environment[80];
    snprintf(environment, sizeof(environment),
             "MEMCACHED_PORT_FILENAME=/tmp/ports.%lu", (long)getpid());
    char *filename= environment + strlen("MEMCACHED_PORT_FILENAME=");
    char pid_file[80];
    snprintf(pid_file, sizeof(pid_file), "/tmp/pid.%lu", (long)getpid());

    remove(filename);
    remove(pid_file);

#ifdef __sun
     
    char coreadm[128];
    sprintf(coreadm, "coreadm -p core.%%f.%%p %lu", (unsigned long)getpid());
    system(coreadm);
#endif

    pid_t pid = fork();
    assert(pid != -1);

    if (pid == 0) {
         
        char *argv[20];
        int arg = 0;
        char tmo[24];
        snprintf(tmo, sizeof(tmo), "%u", timeout);

        putenv(environment);
#ifdef __sun
        putenv("LD_PRELOAD=watchmalloc.so.1");
        putenv("MALLOC_DEBUG=WATCH");
#endif

        if (!daemon) {
            argv[arg++] = "./timedrun";
            argv[arg++] = tmo;
        }
        argv[arg++] = "./memcached-debug";
        argv[arg++] = "-p";
        argv[arg++] = "-1";
        argv[arg++] = "-U";
        argv[arg++] = "0";
         
        if (getuid() == 0) {
            argv[arg++] = "-u";
            argv[arg++] = "root";
        }
        if (daemon) {
            argv[arg++] = "-d";
            argv[arg++] = "-P";
            argv[arg++] = pid_file;
        }
#ifdef MESSAGE_DEBUG
         argv[arg++] = "-vvv";
#endif
        argv[arg++] = NULL;
        assert(execv(argv[0], argv) != -1);
    }

     
    while (access(filename, F_OK) == -1) {
        usleep(10);
    }

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open the file containing port numbers: %s\n",
                strerror(errno));
        assert(false);
    }

    *port_out = (in_port_t)-1;
    char buffer[80];
    while ((fgets(buffer, sizeof(buffer), fp)) != NULL) {
        if (strncmp(buffer, "TCP INET: ", 10) == 0) {
            int32_t val;
            assert(safe_strtol(buffer + 10, &val));
            *port_out = (in_port_t)val;
        }
    }
    fclose(fp);
    assert(remove(filename) == 0);

    if (daemon) {
         
        while (access(pid_file, F_OK) == -1) {
            usleep(10);
        }

        fp = fopen(pid_file, "r");
        if (fp == NULL) {
            fprintf(stderr, "Failed to open pid file: %s\n",
                    strerror(errno));
            assert(false);
        }
        assert(fgets(buffer, sizeof(buffer), fp) != NULL);
        fclose(fp);

        int32_t val;
        assert(safe_strtol(buffer, &val));
        pid = (pid_t)val;
    }

    return pid;
}