int get_ctty_devnr(pid_t pid, dev_t *d) {
        int k;
        char line[LINE_MAX], *p, *fn;
        unsigned long ttynr;
        FILE *f;

        if (asprintf(&fn, "/proc/%lu/stat", (unsigned long) (pid <= 0 ? getpid() : pid)) < 0)
                return -ENOMEM;

        f = fopen(fn, "re");
        free(fn);
        if (!f)
                return -errno;

        if (!fgets(line, sizeof(line), f)) {
                k = feof(f) ? -EIO : -errno;
                fclose(f);
                return k;
        }

        fclose(f);

        p = strrchr(line, ')');
        if (!p)
                return -EIO;

        p++;

        if (sscanf(p, " "
                   "%*c "   
                   "%*d "   
                   "%*d "   
                   "%*d "   
                   "%lu ",  
                   &ttynr) != 1)
                return -EIO;

        *d = (dev_t) ttynr;
        return 0;
}
