char* getlogname_malloc(void) {
        uid_t uid;
        long bufsize;
        char *buf, *name;
        struct passwd pwbuf, *pw = NULL;
        struct stat st;

        if (isatty(STDIN_FILENO) && fstat(STDIN_FILENO, &st) >= 0)
                uid = st.st_uid;
        else
                uid = getuid();

         
        if (uid == 0)
                return strdup("root");

        if ((bufsize = sysconf(_SC_GETPW_R_SIZE_MAX)) <= 0)
                bufsize = 4096;

        if (!(buf = malloc(bufsize)))
                return NULL;

        if (getpwuid_r(uid, &pwbuf, buf, bufsize, &pw) == 0 && pw) {
                name = strdup(pw->pw_name);
                free(buf);
                return name;
        }

        free(buf);

        if (asprintf(&name, "%lu", (unsigned long) uid) < 0)
                return NULL;

        return name;
}
