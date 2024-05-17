int is_kernel_thread(pid_t pid) {
        char *p;
        size_t count;
        char c;
        bool eof;
        FILE *f;

        if (pid == 0)
                return 0;

        if (asprintf(&p, "/proc/%lu/cmdline", (unsigned long) pid) < 0)
                return -ENOMEM;

        f = fopen(p, "re");
        free(p);

        if (!f)
                return -errno;

        count = fread(&c, 1, 1, f);
        eof = feof(f);
        fclose(f);

         

        if (count <= 0)
                return eof ? 1 : -errno;

        return 0;
}
