void Curl_cookie_freelist(struct Cookie *co, bool cookiestoo)
{
  struct Cookie *next;
  if(co) {
    while(co) {
      next = co->next;
      if(cookiestoo)
        freecookie(co);
      else
        free(co);  
      co = next;
    }
  }
}
