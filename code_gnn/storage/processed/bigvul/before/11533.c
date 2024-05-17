int get_parent_of_pid(pid_t pid, pid_t *_ppid) {
        int r;
        FILE *f;
        char fn[PATH_MAX], line[LINE_MAX], *p;
        long unsigned ppid;

        assert(pid > 0);
        assert(_ppid);

        assert_se(snprintf(fn, sizeof(fn)-1, "/proc/%lu/stat", (unsigned long) pid) < (int) (sizeof(fn)-1));
        char_array_0(fn);

        if (!(f = fopen(fn, "re")))
                return -errno;

        if (!(fgets(line, sizeof(line), f))) {
                r = feof(f) ? -EIO : -errno;
                fclose(f);
                return r;
        }

        fclose(f);

         

        if (!(p = strrchr(line, ')')))
                return -EIO;

        p++;

        if (sscanf(p, " "
                   "%*c "   
                   "%lu ",  
                   &ppid) != 1)
                return -EIO;

        if ((long unsigned) (pid_t) ppid != ppid)
                return -ERANGE;

        *_ppid = (pid_t) ppid;

        return 0;
}
