run_handler (const gchar *act, const gchar *args) {
     

    if (!act) return;
    char **parts = g_strsplit(act, " ", 2);
    if (!parts || !parts[0]) return;
    if (g_strcmp0(parts[0], "chain") == 0) {
        GString *newargs = g_string_new("");
        gchar **chainparts = split_quoted(parts[1], FALSE);

         
        gchar **cp = chainparts;
        gchar quot = '\'';
        gchar *quotless = NULL;
        gchar **spliced_quotless = NULL;  
        gchar **inpart = NULL;

        while (*cp) {
            if ((**cp == '\'') || (**cp == '\"')) {  
                quot = **cp;
                quotless = g_strndup(&(*cp)[1], strlen(*cp) - 2);
            } else quotless = g_strdup(*cp);

            spliced_quotless = g_strsplit(quotless, " ", 2);
            inpart = inject_handler_args(spliced_quotless[0], spliced_quotless[1], args);
            g_strfreev(spliced_quotless);

            g_string_append_printf(newargs, " %c%s %s%c", quot, inpart[0], inpart[1], quot);
            g_free(quotless);
            g_strfreev(inpart);
            cp++;
        }

        parse_command(parts[0], &(newargs->str[1]), NULL);
        g_string_free(newargs, TRUE);
        g_strfreev(chainparts);

    } else {
        gchar **inparts;
        gchar *inparts_[2];
        if (parts[1]) {
             
            gchar* expanded = expand(parts[1], 0);
            inparts = inject_handler_args(parts[0], expanded, args);
            g_free(expanded);
        } else {
            inparts_[0] = parts[0];
            inparts_[1] = g_strdup(args);
            inparts = inparts_;
        }

        parse_command(inparts[0], inparts[1], NULL);

        if (inparts != inparts_) {
            g_free(inparts[0]);
            g_free(inparts[1]);
        } else
            g_free(inparts[1]);
    }
    g_strfreev(parts);
}
