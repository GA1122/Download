static int cmpfile(const char *str, const char *path, const char *fname)
{
    char filename[1024], *buffer;
    int ret;
    FILE *file;

    sprintf(filename, "%s%c%s", path, dir_sep, fname);
    file = fopen(filename, "rb");
    if (!file) {
        if (conf.strip)
            strcat(filename, ".strip");
        else
            strcat(filename, ".normal");
        file = fopen(filename, "rb");
    }
    if (!file) {
        printf("Error: test result file could not be opened.\n");
        exit(1);
    }

    buffer = loadfile(file);
    if (strcmp(buffer, str) != 0)
        ret = 1;
    else
        ret = 0;
    free(buffer);
    fclose(file);

    return ret;
}
