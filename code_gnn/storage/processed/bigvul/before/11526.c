bool first_word(const char *s, const char *word) {
        size_t sl, wl;

        assert(s);
        assert(word);

        sl = strlen(s);
        wl = strlen(word);

        if (sl < wl)
                return false;

        if (wl == 0)
                return true;

        if (memcmp(s, word, wl) != 0)
                return false;

        return s[wl] == 0 ||
                strchr(WHITESPACE, s[wl]);
}
