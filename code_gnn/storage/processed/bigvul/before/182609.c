 static int string_check(char *buf, const char *buf2)
// static int _string_check(int linenumber, char *buf, const char *buf2)
  {
    if(strcmp(buf, buf2)) {
       
    printf("sprintf failed:\nwe '%s'\nsystem: '%s'\n",
           buf, buf2);
//     printf("sprintf line %d failed:\nwe      '%s'\nsystem: '%s'\n",
//            linenumber, buf, buf2);
      return 1;
    }
    return 0;
  }