unsigned int arg_parse_uint(const struct arg *arg) {
 long int   rawval;
 char *endptr;

  rawval = strtol(arg->val, &endptr, 10);

 if (arg->val[0] != '\0' && endptr[0] == '\0') {
 if (rawval >= 0 && rawval <= UINT_MAX)
 return rawval;

    die("Option %s: Value %ld out of range for unsigned int\n",
        arg->name, rawval);
 }

  die("Option %s: Invalid character '%c'\n", arg->name, *endptr);
 return 0;
}
