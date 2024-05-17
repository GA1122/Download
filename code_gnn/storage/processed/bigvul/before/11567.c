bool path_is_absolute(const char *p) {
        assert(p);

        return p[0] == '/';
}
