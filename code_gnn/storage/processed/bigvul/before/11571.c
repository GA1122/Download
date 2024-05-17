char *path_make_absolute_cwd(const char *p) {
        char *cwd, *r;

        assert(p);

         

        if (path_is_absolute(p))
                return strdup(p);

        if (!(cwd = get_current_dir_name()))
                return NULL;

        r = path_make_absolute(p, cwd);
        free(cwd);

        return r;
}
