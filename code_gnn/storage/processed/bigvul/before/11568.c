int path_is_mount_point(const char *t, bool allow_symlink) {
        struct stat a, b;
        char *parent;
        int r;

        if (allow_symlink)
                r = stat(t, &a);
        else
                r = lstat(t, &a);

        if (r < 0) {
                if (errno == ENOENT)
                        return 0;

                return -errno;
        }

        r = parent_of_path(t, &parent);
        if (r < 0)
                return r;

        r = lstat(parent, &b);
        free(parent);

        if (r < 0)
                return -errno;

        return a.st_dev != b.st_dev;
}
