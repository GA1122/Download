char *delete_chars(char *s, const char *bad) {
        char *f, *t;

         

        for (f = s, t = s; *f; f++) {
                if (strchr(bad, *f))
                        continue;

                *(t++) = *f;
        }

        *t = 0;

        return s;
}
