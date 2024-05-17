char *path_make_absolute(const char *p, const char *prefix) {
        assert(p);

         

        if (path_is_absolute(p) || !prefix)
                return strdup(p);

        return join(prefix, "/", p, NULL);
}
