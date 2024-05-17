  static int is_integer(char *string)
  {
  if (isdigit(string[0]) || string[0] == '-' || string[0] == '+') {
    while (*++string && isdigit(*string))
//   if (isdigit((unsigned char) string[0]) || string[0] == '-' || string[0] == '+') {
//     while (*++string && isdigit((unsigned char) *string))
        ;                                            
      if (!*string)
        return 1;
   }
   return 0;
 }