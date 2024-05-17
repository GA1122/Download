const char *arg_next(struct arg *arg) {
 if (arg->argv[0])
    arg->argv += arg->argv_step;

 return *arg->argv;
}
