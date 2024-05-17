int arg_parse_enum_or_int(const struct arg *arg) {
 if (arg->def->enums)
 return arg_parse_enum(arg);
 return arg_parse_int(arg);
}
