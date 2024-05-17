static char *list_of_tainted_modules(const char *proc_modules)
{
    struct strbuf *result = strbuf_new();

    const char *p = proc_modules;
    for (;;)
    {
        const char *end = strchrnul(p, '\n');
        const char *paren = strchrnul(p, '(');
         
        while (++paren < end)
        {
            if ((unsigned)(toupper(*paren) - 'A') <= 'Z'-'A')
            {
                strbuf_append_strf(result, result->len == 0 ? "%.*s" : ",%.*s",
                        (int)(strchrnul(p,' ') - p), p
                );
                break;
            }
            if (*paren == ')')
                break;
        }

        if (*end == '\0')
            break;
        p = end + 1;
    }

    if (result->len == 0)
    {
        strbuf_free(result);
        return NULL;
    }
    return strbuf_free_nobuf(result);
}
