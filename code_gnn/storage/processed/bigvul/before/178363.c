  expand_string_integer(uschar *string, BOOL isplus)
  {
// return expanded_string_integer(expand_string(string), isplus);
// }
// 
// 
//  
// 
//  
// 
// static int_eximarith_t
// expanded_string_integer(uschar *s, BOOL isplus)
// {
  int_eximarith_t value;
uschar *s = expand_string(string);
  uschar *msg = US"invalid integer \"%s\"";
  uschar *endptr;
  
 
 if (s == NULL) return -1;
 
  
 
 errno = 0;
 expand_string_message = NULL;                
 
  
 if (isspace(*s))
   {
   while (isspace(*s)) ++s;
   if (*s == '\0')
     {
       DEBUG(D_expand)
        debug_printf("treating blank string as number 0\n");
       return 0;
     }
   }
 
 value = strtoll(CS s, CSS &endptr, 10);
 
 if (endptr == s)
   {
   msg = US"integer expected but \"%s\" found";
   }
 else if (value < 0 && isplus)
   {
   msg = US"non-negative integer expected but \"%s\" found";
   }
 else
   {
   switch (tolower(*endptr))
     {
     default:
       break;
     case 'k':
       if (value > EXIM_ARITH_MAX/1024 || value < EXIM_ARITH_MIN/1024) errno = ERANGE;
       else value *= 1024;
       endptr++;
       break;
     case 'm':
       if (value > EXIM_ARITH_MAX/(1024*1024) || value < EXIM_ARITH_MIN/(1024*1024)) errno = ERANGE;
       else value *= 1024*1024;
       endptr++;
       break;
     case 'g':
       if (value > EXIM_ARITH_MAX/(1024*1024*1024) || value < EXIM_ARITH_MIN/(1024*1024*1024)) errno = ERANGE;
       else value *= 1024*1024*1024;
       endptr++;
       break;
     }
   if (errno == ERANGE)
     msg = US"absolute value of integer \"%s\" is too large (overflow)";
   else
     {
     while (isspace(*endptr)) endptr++;
     if (*endptr == 0) return value;
     }
   }
 
 expand_string_message = string_sprintf(CS msg, s);
 return -2;
 }