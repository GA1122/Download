char **argv_dup(int argc, const char **argv) {
 char **new_argv = malloc((argc + 1) * sizeof(*argv));

  memcpy(new_argv, argv, argc * sizeof(*argv));
  new_argv[argc] = NULL;
 return new_argv;
}
