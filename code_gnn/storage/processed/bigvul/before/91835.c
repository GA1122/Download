char *strdup(char *s)
{
    char *retval;

    retval = (char *) malloc(strlen(s) + 1);
    if (retval == NULL) {
        perror("boa: out of memory in strdup");
        exit(1);
    }
    return strcpy(retval, s);
}
