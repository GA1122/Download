int main(int argc, char *argv[])
{
    int i;
    char *test_path = NULL;

    #ifdef HAVE_SETLOCALE
    setlocale(LC_ALL, "");
    #endif

    if (argc < 2) {
        goto usage;
    }

    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--strip"))
            conf.strip = 1;
        else if (!strcmp(argv[i], "--env"))
            conf.use_env = 1;
        else
            test_path = argv[i];
    }

    if (conf.use_env)
        return use_env();
    else
    {
        if (!test_path)
            goto usage;

        return use_conf(test_path);
    }

usage:
    fprintf(stderr, "argc =%d\n", argc);
    fprintf(stderr, "usage: %s [--strip] [--env] test_dir\n", argv[0]);
    return 2;
}
