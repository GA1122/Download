static char *resultFilename(const char *filename, const char *out,
                            const char *suffix) {
    const char *base;
    char res[500];
    char suffixbuff[500];

 

    base = baseFilename(filename);
    if (suffix == NULL)
        suffix = ".tmp";
    if (out == NULL)
        out = "";

    strncpy(suffixbuff,suffix,499);
#ifdef VMS
    if(strstr(base,".") && suffixbuff[0]=='.')
      suffixbuff[0]='_';
#endif

    snprintf(res, 499, "%s%s%s", out, base, suffixbuff);
    res[499] = 0;
    return(strdup(res));
}
