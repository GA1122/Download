int readlink_and_make_absolute(const char *p, char **r) {
        char *target, *k;
        int j;

        assert(p);
        assert(r);

        if ((j = readlink_malloc(p, &target)) < 0)
                return j;

        k = file_in_same_dir(p, target);
        free(target);

        if (!k)
                return -ENOMEM;

        *r = k;
        return 0;
}
