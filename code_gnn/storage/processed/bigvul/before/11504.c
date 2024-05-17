bool chars_intersect(const char *a, const char *b) {
        const char *p;

         
        for (p = a; *p; p++)
                if (strchr(b, *p))
                        return true;

        return false;
}
