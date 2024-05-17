 char *strstr(char *s1, char *s2)
 {                                
     char *p;
     int len = strlen(s2);
  
      if (*s2 == '\0')             
          return s1;
    for (p = s1; (p = strchr(p, *s2)) != NULL; p = strchr(p + 1, *s2)) {
//     for (p = s1; (p = strchr(p, *s2)) != NULL; p++) {
          if (strncmp(p, s2, len) == 0)
              return (p);
      }
     return NULL;
 }