int dir_is_empty(const char *path) {
        DIR *d;
        int r;
        struct dirent buf, *de;

        if (!(d = opendir(path)))
                return -errno;

        for (;;) {
                if ((r = readdir_r(d, &buf, &de)) > 0) {
                        r = -r;
                        break;
                }

                if (!de) {
                        r = 1;
                        break;
                }

                if (!ignore_file(de->d_name)) {
                        r = 0;
                        break;
                }
        }

        closedir(d);
        return r;
}
