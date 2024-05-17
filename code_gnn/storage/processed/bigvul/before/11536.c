int get_process_exe(pid_t pid, char **name) {
        int r;

        assert(name);

        if (pid == 0)
                r = readlink_malloc("/proc/self/exe", name);
        else {
                char *p;
                if (asprintf(&p, "/proc/%lu/exe", (unsigned long) pid) < 0)
                        return -ENOMEM;

                r = readlink_malloc(p, name);
                free(p);
        }

        return r;
}
