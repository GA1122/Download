static char *trim(char *str) {
 while (isspace(*str))
 ++str;

 if (!*str)
 return str;

 char *end_str = str + strlen(str) - 1;
 while (end_str > str && isspace(*end_str))
 --end_str;

  end_str[1] = '\0';
 return str;
}
