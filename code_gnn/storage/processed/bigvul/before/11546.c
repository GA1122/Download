bool in_charset(const char *s, const char* charset) {
        const char *i;

        assert(s);
        assert(charset);

        for (i = s; *i; i++)
                if (!strchr(charset, *i))
                        return false;

        return true;
}
