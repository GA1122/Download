int get_starttime_of_pid(pid_t pid, unsigned long long *st) {
        int r;
        FILE *f;
        char fn[PATH_MAX], line[LINE_MAX], *p;

        assert(pid > 0);
        assert(st);

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
                   "%*d "   
                   "%*d "   
                   "%*d "   
                   "%*d "   
                   "%*d "   
                   "%*u "   
                   "%*u "   
                   "%*u "   
                   "%*u "   
                   "%*u "   
                   "%*u "   
                   "%*u "   
                   "%*d "   
                   "%*d "   
                   "%*d "   
                   "%*d "   
                   "%*d "   
                   "%*d "   
                   "%llu "   ,
                   st) != 1)
                return -EIO;

        return 0;
}
