Curl_cookie_add(struct SessionHandle *data,
                 

                struct CookieInfo *c,
                bool httpheader,  
                char *lineptr,    
                const char *domain,  
                const char *path)    
{
  struct Cookie *clist;
  char name[MAX_NAME];
  struct Cookie *co;
  struct Cookie *lastc=NULL;
  time_t now = time(NULL);
  bool replace_old = FALSE;
  bool badcookie = FALSE;  

#ifdef CURL_DISABLE_VERBOSE_STRINGS
  (void)data;
#endif

   
  co = calloc(1, sizeof(struct Cookie));
  if(!co)
    return NULL;  

  if(httpheader) {
     
    const char *ptr;
    const char *semiptr;
    char *what;

    what = malloc(MAX_COOKIE_LINE);
    if(!what) {
      free(co);
      return NULL;
    }

    semiptr=strchr(lineptr, ';');  

    while(*lineptr && ISBLANK(*lineptr))
      lineptr++;

    ptr = lineptr;
    do {
       
      name[0]=what[0]=0;  
      if(1 <= sscanf(ptr, "%" MAX_NAME_TXT "[^;\r\n =]=%"
                     MAX_COOKIE_LINE_TXT "[^;\r\n]",
                     name, what)) {
         
        const char *whatptr;
        bool done = FALSE;
        bool sep;
        size_t len=strlen(what);
        const char *endofn = &ptr[ strlen(name) ];

         
        while(*endofn && ISBLANK(*endofn))
          endofn++;

         
        sep = (*endofn == '=')?TRUE:FALSE;

         
        while(len && ISBLANK(what[len-1])) {
          what[len-1]=0;
          len--;
        }

         
        whatptr=what;
        while(*whatptr && ISBLANK(*whatptr))
          whatptr++;

        if(!len) {
           
          done = TRUE;
          if(Curl_raw_equal("secure", name))
            co->secure = TRUE;
          else if(Curl_raw_equal("httponly", name))
            co->httponly = TRUE;
          else if(sep)
             
            done = FALSE;
        }
        if(done)
          ;
        else if(Curl_raw_equal("path", name)) {
          strstore(&co->path, whatptr);
          if(!co->path) {
            badcookie = TRUE;  
            break;
          }
        }
        else if(Curl_raw_equal("domain", name)) {
           

          const char *domptr=whatptr;
          const char *nextptr;
          int dotcount=1;

           

          if('.' == whatptr[0])
             
            domptr++;

          do {
            nextptr = strchr(domptr, '.');
            if(nextptr) {
              if(domptr != nextptr)
                dotcount++;
              domptr = nextptr+1;
            }
          } while(nextptr);

           

          if(dotcount < 2) {
             
            badcookie=TRUE;  
            infof(data, "skipped cookie with illegal dotcount domain: %s\n",
                  whatptr);
          }
          else {
             

            if('.' == whatptr[0])
              whatptr++;  

            if(!domain || tailmatch(whatptr, domain)) {
              const char *tailptr=whatptr;
              if(tailptr[0] == '.')
                tailptr++;
              strstore(&co->domain, tailptr);  
              if(!co->domain) {
                badcookie = TRUE;
                break;
              }
              co->tailmatch=TRUE;  
            }
            else {
               
              badcookie=TRUE;
              infof(data, "skipped cookie with bad tailmatch domain: %s\n",
                    whatptr);
            }
          }
        }
        else if(Curl_raw_equal("version", name)) {
          strstore(&co->version, whatptr);
          if(!co->version) {
            badcookie = TRUE;
            break;
          }
        }
        else if(Curl_raw_equal("max-age", name)) {
           
          strstore(&co->maxage, whatptr);
          if(!co->maxage) {
            badcookie = TRUE;
            break;
          }
          co->expires =
            strtol((*co->maxage=='\"')?&co->maxage[1]:&co->maxage[0],NULL,10)
            + (long)now;
        }
        else if(Curl_raw_equal("expires", name)) {
          strstore(&co->expirestr, whatptr);
          if(!co->expirestr) {
            badcookie = TRUE;
            break;
          }
           
          co->expires = curl_getdate(what, &now);

           
          if(co->expires == 0)
            co->expires = 1;
          else if(co->expires < 0)
            co->expires = 0;
        }
        else if(!co->name) {
          co->name = strdup(name);
          co->value = strdup(whatptr);
          if(!co->name || !co->value) {
            badcookie = TRUE;
            break;
          }
        }
         
      }
      else {
         
      }

      if(!semiptr || !*semiptr) {
         
        semiptr = NULL;
        continue;
      }

      ptr=semiptr+1;
      while(*ptr && ISBLANK(*ptr))
        ptr++;
      semiptr=strchr(ptr, ';');  

      if(!semiptr && *ptr)
         
        semiptr=strchr(ptr, '\0');
    } while(semiptr);

    if(!badcookie && !co->domain) {
      if(domain) {
         
        co->domain=strdup(domain);
        if(!co->domain)
          badcookie = TRUE;
      }
    }

    if(!badcookie && !co->path && path) {
       
      char *queryp = strchr(path, '?');

       
      char *endslash;
      if(!queryp)
        endslash = strrchr(path, '/');
      else
        endslash = memrchr(path, '/', (size_t)(queryp - path));
      if(endslash) {
        size_t pathlen = (size_t)(endslash-path+1);  
        co->path=malloc(pathlen+1);  
        if(co->path) {
          memcpy(co->path, path, pathlen);
          co->path[pathlen]=0;  
        }
        else
          badcookie = TRUE;
      }
    }

    free(what);

    if(badcookie || !co->name) {
       
      freecookie(co);
      return NULL;
    }

  }
  else {
     
    char *ptr;
    char *firstptr;
    char *tok_buf=NULL;
    int fields;

     
    if(strncmp(lineptr, "#HttpOnly_", 10) == 0) {
      lineptr += 10;
      co->httponly = TRUE;
    }

    if(lineptr[0]=='#') {
       
      free(co);
      return NULL;
    }
     
    ptr=strchr(lineptr, '\r');
    if(ptr)
      *ptr=0;  
    ptr=strchr(lineptr, '\n');
    if(ptr)
      *ptr=0;  

    firstptr=strtok_r(lineptr, "\t", &tok_buf);  

     
    if(!firstptr || strchr(firstptr, ':')) {
      free(co);
      return NULL;
    }

     
    for(ptr=firstptr, fields=0; ptr && !badcookie;
        ptr=strtok_r(NULL, "\t", &tok_buf), fields++) {
      switch(fields) {
      case 0:
        if(ptr[0]=='.')  
          ptr++;
        co->domain = strdup(ptr);
        if(!co->domain)
          badcookie = TRUE;
        break;
      case 1:
         
        co->tailmatch = Curl_raw_equal(ptr, "TRUE")?TRUE:FALSE;
        break;
      case 2:
         
        if(strcmp("TRUE", ptr) && strcmp("FALSE", ptr)) {
           
          co->path = strdup(ptr);
          if(!co->path)
            badcookie = TRUE;
          break;
        }
         
        co->path = strdup("/");
        if(!co->path)
          badcookie = TRUE;
        fields++;  
         
      case 3:
        co->secure = Curl_raw_equal(ptr, "TRUE")?TRUE:FALSE;
        break;
      case 4:
        co->expires = curlx_strtoofft(ptr, NULL, 10);
        break;
      case 5:
        co->name = strdup(ptr);
        if(!co->name)
          badcookie = TRUE;
        break;
      case 6:
        co->value = strdup(ptr);
        if(!co->value)
          badcookie = TRUE;
        break;
      }
    }
    if(6 == fields) {
       
      co->value = strdup("");
      if(!co->value)
        badcookie = TRUE;
      else
        fields++;
    }

    if(!badcookie && (7 != fields))
       
      badcookie = TRUE;

    if(badcookie) {
      freecookie(co);
      return NULL;
    }

  }

  if(!c->running &&     
     c->newsession &&   
     !co->expires) {    
    freecookie(co);
    return NULL;
  }

  co->livecookie = c->running;

   

  clist = c->cookies;
  replace_old = FALSE;
  while(clist) {
    if(Curl_raw_equal(clist->name, co->name)) {
       

      if(clist->domain && co->domain) {
        if(Curl_raw_equal(clist->domain, co->domain))
           
          replace_old=TRUE;
      }
      else if(!clist->domain && !co->domain)
        replace_old = TRUE;

      if(replace_old) {
         

        if(clist->path && co->path) {
          if(Curl_raw_equal(clist->path, co->path)) {
            replace_old = TRUE;
          }
          else
            replace_old = FALSE;
        }
        else if(!clist->path && !co->path)
          replace_old = TRUE;
        else
          replace_old = FALSE;

      }

      if(replace_old && !co->livecookie && clist->livecookie) {
         

         
        freecookie(co);
        return NULL;
      }

      if(replace_old) {
        co->next = clist->next;  

         
        free(clist->name);
        if(clist->value)
          free(clist->value);
        if(clist->domain)
          free(clist->domain);
        if(clist->path)
          free(clist->path);
        if(clist->expirestr)
          free(clist->expirestr);

        if(clist->version)
          free(clist->version);
        if(clist->maxage)
          free(clist->maxage);

        *clist = *co;   

        free(co);    
        co = clist;  

         
        do {
          lastc = clist;
          clist = clist->next;
        } while(clist);
        break;
      }
    }
    lastc = clist;
    clist = clist->next;
  }

  if(c->running)
     
    infof(data, "%s cookie %s=\"%s\" for domain %s, path %s, "
          "expire %" FORMAT_OFF_T "\n",
          replace_old?"Replaced":"Added", co->name, co->value,
          co->domain, co->path, co->expires);

  if(!replace_old) {
     
    if(lastc)
      lastc->next = co;
    else
      c->cookies = co;
    c->numcookies++;  
  }

  return co;
}
