 emit_string(const char *str, FILE *out)
   
 {
  for (; *str; ++str)
  if (isgraph(UCHAR_MAX & *str))
          putc(*str, out);
 
  
        else if (isspace(UCHAR_MAX & *str))
           putc('_', out);
// 
        else
           fprintf(out, "\\%.3o", *str);
  }