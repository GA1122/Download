int mkdir_parents(const char *path, mode_t mode) {
        const char *p, *e;

        assert(path);

         

        p = path + strspn(path, "/");
        for (;;) {
                int r;
                char *t;

                e = p + strcspn(p, "/");
                p = e + strspn(e, "/");

                 
                if (*p == 0)
                        return 0;

                if (!(t = strndup(path, e - path)))
                        return -ENOMEM;

                r = label_mkdir(t, mode);
                free(t);

                if (r < 0 && errno != EEXIST)
                        return -errno;
        }
}
