int arg_parse_enum(const struct arg *arg) {
 const struct arg_enum_list *listptr;
 long int                    rawval;
 char *endptr;

  
  rawval = strtol(arg->val, &endptr, 10);
 if (arg->val[0] != '\0' && endptr[0] == '\0') {
  
 for (listptr = arg->def->enums; listptr->name; listptr++)
 if (listptr->val == rawval)
 return rawval;
 }

  
 for (listptr = arg->def->enums; listptr->name; listptr++)
 if (!strcmp(arg->val, listptr->name))
 return listptr->val;

  die("Option %s: Invalid value '%s'\n", arg->name, arg->val);
 return 0;
}
