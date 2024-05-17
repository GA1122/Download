parse_value(const char *s, const char *delimiters)
{
    size_t n = 0;

     
    while (!strchr(delimiters, s[n])) {
        if (s[n] == '(') {
            int level = 0;
            do {
                switch (s[n]) {
                case '\0':
                    return n;
                case '(':
                    level++;
                    break;
                case ')':
                    level--;
                    break;
                }
                n++;
            } while (level > 0);
        } else {
            n++;
        }
    }
    return n;
}
