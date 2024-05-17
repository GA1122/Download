struct CookieInfo *Curl_cookie_init(struct SessionHandle *data,
                                    const char *file,
                                    struct CookieInfo *inc,
                                    bool newsession)
{
  struct CookieInfo *c;
  FILE *fp;
  bool fromfile=TRUE;

  if(NULL == inc) {
     
    c = calloc(1, sizeof(struct CookieInfo));
    if(!c)
      return NULL;  
    c->filename = strdup(file?file:"none");  
  }
  else {
     
    c = inc;
  }
  c->running = FALSE;  

  if(file && strequal(file, "-")) {
    fp = stdin;
    fromfile=FALSE;
  }
  else if(file && !*file) {
     
    fp = NULL;
  }
  else
    fp = file?fopen(file, "r"):NULL;

  c->newsession = newsession;  

  if(fp) {
    char *lineptr;
    bool headerline;

    char *line = malloc(MAX_COOKIE_LINE);
    if(line) {
      while(fgets(line, MAX_COOKIE_LINE, fp)) {
        if(checkprefix("Set-Cookie:", line)) {
           
          lineptr=&line[11];
          headerline=TRUE;
        }
        else {
          lineptr=line;
          headerline=FALSE;
        }
        while(*lineptr && ISBLANK(*lineptr))
          lineptr++;

        Curl_cookie_add(data, c, headerline, lineptr, NULL, NULL);
      }
      free(line);  
    }
    if(fromfile)
      fclose(fp);
  }

  c->running = TRUE;           

  return c;
}
