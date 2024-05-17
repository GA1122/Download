int arg_match(struct arg *arg_, const struct arg_def *def, char **argv) {
 struct arg arg;

 if (!argv[0] || argv[0][0] != '-')
 return 0;

  arg = arg_init(argv);

 if (def->short_name
 && strlen(arg.argv[0]) == strlen(def->short_name) + 1
 && !strcmp(arg.argv[0] + 1, def->short_name)) {

    arg.name = arg.argv[0] + 1;
    arg.val = def->has_val ? arg.argv[1] : NULL;
    arg.argv_step = def->has_val ? 2 : 1;
 } else if (def->long_name) {
 const size_t name_len = strlen(def->long_name);

 if (strlen(arg.argv[0]) >= name_len + 2
 && arg.argv[0][1] == '-'
 && !strncmp(arg.argv[0] + 2, def->long_name, name_len)
 && (arg.argv[0][name_len + 2] == '='
 || arg.argv[0][name_len + 2] == '\0')) {

      arg.name = arg.argv[0] + 2;
      arg.val = arg.name[name_len] == '=' ? arg.name + name_len + 1 : NULL;
      arg.argv_step = 1;
 }
 }

 if (arg.name && !arg.val && def->has_val)
    die("Error: option %s requires argument.\n", arg.name);

 if (arg.name && arg.val && !def->has_val)
    die("Error: option %s requires no argument.\n", arg.name);

 if (arg.name
 && (arg.val || !def->has_val)) {
    arg.def = def;
 *arg_ = arg;
 return 1;
 }

 return 0;
}
