struct Cookie *Curl_cookie_getlist(struct CookieInfo *c,
                                   const char *host, const char *path,
                                   bool secure)
{
  struct Cookie *newco;
  struct Cookie *co;
  time_t now = time(NULL);
  struct Cookie *mainco=NULL;
  size_t matches = 0;

  if(!c || !c->cookies)
    return NULL;  

  co = c->cookies;

  while(co) {
     
    if((!co->expires || (co->expires > now)) &&
       (co->secure?secure:TRUE)) {

       
      if(!co->domain ||
         (co->tailmatch && tailmatch(co->domain, host)) ||
         (!co->tailmatch && Curl_raw_equal(host, co->domain)) ) {
         

         
        if(!co->path ||
            
           !strncmp(co->path, path, strlen(co->path)) ) {

           

          newco = malloc(sizeof(struct Cookie));
          if(newco) {
             
            memcpy(newco, co, sizeof(struct Cookie));

             
            newco->next = mainco;

             
            mainco = newco;

            matches++;
          }
          else {
            fail:
             
            while(mainco) {
              co = mainco->next;
              free(mainco);
              mainco = co;
            }

            return NULL;
          }
        }
      }
    }
    co = co->next;
  }

  if(matches) {
     
    struct Cookie **array;
    size_t i;

     
    array = malloc(sizeof(struct Cookie *) * matches);
    if(!array)
      goto fail;

    co = mainco;

    for(i=0; co; co = co->next)
      array[i++] = co;

     
    qsort(array, matches, sizeof(struct Cookie *), cookie_sort);

     

    mainco = array[0];  
    for(i=0; i<matches-1; i++)
      array[i]->next = array[i+1];
    array[matches-1]->next = NULL;  

    free(array);  
  }

  return mainco;  
}
