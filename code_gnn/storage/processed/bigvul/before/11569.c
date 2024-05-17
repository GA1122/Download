char *path_kill_slashes(char *path) {
        char *f, *t;
        bool slash = false;

         

        for (f = path, t = path; *f; f++) {

                if (*f == '/') {
                        slash = true;
                        continue;
                }

                if (slash) {
                        slash = false;
                        *(t++) = '/';
                }

                *(t++) = *f;
        }

         

        if (t == path && slash)
                *(t++) = '/';

        *t = 0;
        return path;
}
