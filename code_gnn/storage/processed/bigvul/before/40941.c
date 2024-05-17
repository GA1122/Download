static char *loadfile(FILE *file)
{
    long fsize, ret;
    char *buf;

    fseek(file, 0, SEEK_END);
    fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    buf = malloc(fsize+1);
    ret = fread(buf, 1, fsize, file);
    if (ret != fsize)
        exit(1);
    buf[fsize] = '\0';

    return buf;
}
