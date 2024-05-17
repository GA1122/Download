struct vpx_rational arg_parse_rational(const struct arg *arg) {
 long int             rawval;
 char *endptr;
 struct vpx_rational  rat;

  
  rawval = strtol(arg->val, &endptr, 10);

 if (arg->val[0] != '\0' && endptr[0] == '/') {
 if (rawval >= INT_MIN && rawval <= INT_MAX)
      rat.num = rawval;
 else die("Option %s: Value %ld out of range for signed int\n",
               arg->name, rawval);
 } else die("Option %s: Expected / at '%c'\n", arg->name, *endptr);

  
  rawval = strtol(endptr + 1, &endptr, 10);

 if (arg->val[0] != '\0' && endptr[0] == '\0') {
 if (rawval >= INT_MIN && rawval <= INT_MAX)
      rat.den = rawval;
 else die("Option %s: Value %ld out of range for signed int\n",
               arg->name, rawval);
 } else die("Option %s: Invalid character '%c'\n", arg->name, *endptr);

 return rat;
}
