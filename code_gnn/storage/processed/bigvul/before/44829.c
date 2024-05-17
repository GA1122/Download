local void show_info(int method, unsigned long check, off_t len, int cont)
{
    size_t max;              
    size_t n;                
    time_t now;              
    char mod[26];            
    char tag[NAMEMAX1+1];    

     
    max = g.verbosity > 1 ? NAMEMAX2 : NAMEMAX1;
    memset(tag, 0, max + 1);
    if (cont)
        strncpy(tag, "<...>", max + 1);
    else if (g.hname == NULL) {
        n = strlen(g.inf) - compressed_suffix(g.inf);
        strncpy(tag, g.inf, n > max + 1 ? max + 1 : n);
        if (strcmp(g.inf + n, ".tgz") == 0 && n < max + 1)
            strncpy(tag + n, ".tar", max + 1 - n);
    }
    else
        strncpy(tag, g.hname, max + 1);
    if (tag[max])
        strcpy(tag + max - 3, "...");

     
    if (g.stamp) {
        strcpy(mod, ctime(&g.stamp));
        now = time(NULL);
        if (strcmp(mod + 20, ctime(&now) + 20) != 0)
            strcpy(mod + 11, mod + 19);
    }
    else
        strcpy(mod + 4, "------ -----");
    mod[16] = 0;

     
    if (g.first) {
        if (g.verbosity > 1)
            fputs("method    check    timestamp    ", stdout);
        if (g.verbosity > 0)
            puts("compressed   original reduced  name");
        g.first = 0;
    }

     
    if (g.verbosity > 1) {
        if (g.form == 3 && !g.decode)
            printf("zip%3d  --------  %s  ", method, mod + 4);
        else if (g.form > 1)
            printf("zip%3d  %08lx  %s  ", method, check, mod + 4);
        else if (g.form == 1)
            printf("zlib%2d  %08lx  %s  ", method, check, mod + 4);
        else if (method == 257)
            printf("lzw     --------  %s  ", mod + 4);
        else
            printf("gzip%2d  %08lx  %s  ", method, check, mod + 4);
    }
    if (g.verbosity > 0) {
        if ((g.form == 3 && !g.decode) ||
            (method == 8 && g.in_tot > (len + (len >> 10) + 12)) ||
            (method == 257 && g.in_tot > len + (len >> 1) + 3))
#if __STDC_VERSION__-0 >= 199901L || __GNUC__-0 >= 3
            printf("%10jd %10jd?  unk    %s\n",
                   (intmax_t)g.in_tot, (intmax_t)len, tag);
        else
            printf("%10jd %10jd %6.1f%%  %s\n",
                   (intmax_t)g.in_tot, (intmax_t)len,
                   len == 0 ? 0 : 100 * (len - g.in_tot)/(double)len,
                   tag);
#else
            printf(sizeof(off_t) == sizeof(long) ?
                   "%10ld %10ld?  unk    %s\n" : "%10lld %10lld?  unk    %s\n",
                   g.in_tot, len, tag);
        else
            printf(sizeof(off_t) == sizeof(long) ?
                   "%10ld %10ld %6.1f%%  %s\n" : "%10lld %10lld %6.1f%%  %s\n",
                   g.in_tot, len,
                   len == 0 ? 0 : 100 * (len - g.in_tot)/(double)len,
                   tag);
#endif
    }
}
