int get_process_comm(pid_t pid, char **name) {
        int r;

        assert(name);

        if (pid == 0)
                r = read_one_line_file("/proc/self/comm", name);
        else {
                char *p;
                if (asprintf(&p, "/proc/%lu/comm", (unsigned long) pid) < 0)
                        return -ENOMEM;

                r = read_one_line_file(p, name);
                free(p);
        }

        return r;
}
