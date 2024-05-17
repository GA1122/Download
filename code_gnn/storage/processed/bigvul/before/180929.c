 static int mboxlist_do_find(struct find_rock *rock, const strarray_t *patterns)
 {
     const char *userid = rock->userid;
     int isadmin = rock->isadmin;
 
     int crossdomains = config_getswitch(IMAPOPT_CROSSDOMAINS);
     char inbox[MAX_MAILBOX_BUFFER];
     size_t inboxlen = 0;
     size_t prefixlen, len;
     size_t domainlen = 0;
     size_t userlen = userid ? strlen(userid) : 0;
     char domainpat[MAX_MAILBOX_BUFFER];  
     char commonpat[MAX_MAILBOX_BUFFER];
     int r = 0;
     int i;
     const char *p;
 
     if (patterns->count < 1) return 0;  
 
     for (i = 0; i < patterns->count; i++) {
         glob *g = glob_init(strarray_nth(patterns, i), rock->namespace->hier_sep);
         ptrarray_append(&rock->globs, g);
     }
 
     if (config_virtdomains && userid && (p = strchr(userid, '@'))) {
         userlen = p - userid;
         domainlen = strlen(p);  
         snprintf(domainpat, sizeof(domainpat), "%s!", p+1);
     }
     else
         domainpat[0] = '\0';
 
      
     if (userid && (!(p = strchr(userid, rock->namespace->hier_sep)) ||
         ((p - userid) > (int)userlen)) &&
         strlen(userid)+7 < MAX_MAILBOX_BUFFER) {
         char *t, *tmpuser = NULL;
         const char *inboxuser;
 
         if (domainlen)
             snprintf(inbox, sizeof(inbox), "%s!", userid+userlen+1);
         if (rock->namespace->hier_sep == '/' && (p = strchr(userid, '.'))) {
             tmpuser = xmalloc(userlen);
             memcpy(tmpuser, userid, userlen);
             t = tmpuser + (p - userid);
             while(t < (tmpuser + userlen)) {
                 if (*t == '.')
                     *t = '^';
                 t++;
             }
             inboxuser = tmpuser;
         } else
             inboxuser = userid;
         snprintf(inbox+domainlen, sizeof(inbox)-domainlen,
                  "user.%.*s.INBOX.", (int)userlen, inboxuser);
         free(tmpuser);
         inboxlen = strlen(inbox) - 7;
     }
     else {
         userid = 0;
     }
 
      
     const char *firstpat = strarray_nth(patterns, 0);
     for (prefixlen = 0; firstpat[prefixlen]; prefixlen++) {
         if (prefixlen >= MAX_MAILBOX_NAME) {
             r = IMAP_MAILBOX_BADNAME;
             goto done;
         }
         char c = firstpat[prefixlen];
         for (i = 1; i < patterns->count; i++) {
             const char *pat = strarray_nth(patterns, i);
             if (pat[prefixlen] != c) break;
         }
         if (i < patterns->count) break;
         if (c == '*' || c == '%' || c == '?') break;
         commonpat[prefixlen] = c;
     }
     commonpat[prefixlen] = '\0';
 
     if (patterns->count == 1) {
          
         if (!strcmp(firstpat+prefixlen, "%"))
             rock->singlepercent = 2;
          
         if (!strcmp(firstpat+prefixlen, "*%"))
             rock->singlepercent = 1;
     }
 
      
     if (userid && !isadmin) {
          
         rock->mb_category = MBNAME_INBOX;
         r = cyrusdb_forone(rock->db, inbox, inboxlen, &find_p, &find_cb, rock, NULL);
         if (r == CYRUSDB_DONE) r = 0;
         if (r) goto done;
 
         if (rock->namespace->isalt) {
              
             rock->mb_category = MBNAME_INBOXSUB;
             r = cyrusdb_foreach(rock->db, inbox, inboxlen+7, &find_p, &find_cb, rock, NULL);
             if (r == CYRUSDB_DONE) r = 0;
             if (r) goto done;
 
              
             r = (*rock->proc)(NULL, rock->procrock);
             if (r) goto done;
         }
 
          
         rock->mb_category = MBNAME_OWNER;
         r = cyrusdb_foreach(rock->db, inbox, inboxlen+1, &find_p, &find_cb, rock, NULL);
         if (r == CYRUSDB_DONE) r = 0;
         if (r) goto done;
 
          
         if (rock->namespace->isalt) {
              
             r = (*rock->proc)(NULL, rock->procrock);
             if (r) goto done;
 
             rock->mb_category = MBNAME_ALTINBOX;
 
              
             r = cyrusdb_forone(rock->db, inbox, inboxlen+6, &find_p, &find_cb, rock, NULL);
             if (r == CYRUSDB_DONE) goto skipalt;
             if (r) goto done;
 
              
             rock->mb_category = MBNAME_ALTPREFIX;
             r = cyrusdb_foreach(rock->db, inbox, inboxlen+1, &find_p, &find_cb, rock, NULL);
         skipalt:  
             if (r == CYRUSDB_DONE) r = 0;
             if (r) goto done;
         }
     }
 
      
     if (isadmin || rock->namespace->accessible[NAMESPACE_USER]) {
         len = strlen(rock->namespace->prefix[NAMESPACE_USER]);
          if (len) len--;  
  
          if (!strncmp(rock->namespace->prefix[NAMESPACE_USER], commonpat, MIN(len, prefixlen))) {
            if (prefixlen < len) {
//             if (prefixlen <= len) {
                   
                  strlcpy(domainpat+domainlen, "user.", sizeof(domainpat)-domainlen);
              }
             else {
                  
                 strlcpy(domainpat+domainlen, "user.", sizeof(domainpat)-domainlen);
                 strlcpy(domainpat+domainlen+5, commonpat+len+1, sizeof(domainpat)-domainlen-5);
             }
 
             rock->mb_category = MBNAME_OTHERUSER;
 
              
             size_t thislen = (isadmin || crossdomains) ? 0 : strlen(domainpat);
 
              
             r = (*rock->proc)(NULL, rock->procrock);
             if (r) goto done;
 
             r = mboxlist_find_category(rock, domainpat, thislen);
             if (r) goto done;
         }
     }
 
      
     if (isadmin || rock->namespace->accessible[NAMESPACE_SHARED]) {
         len = strlen(rock->namespace->prefix[NAMESPACE_SHARED]);
         if (len) len--;  
 
         if (!strncmp(rock->namespace->prefix[NAMESPACE_SHARED], commonpat, MIN(len, prefixlen))) {
             rock->mb_category = MBNAME_SHARED;
 
              
             r = (*rock->proc)(NULL, rock->procrock);
             if (r) goto done;
 
              
             r = mboxlist_find_category(rock, domainpat, domainlen);
             if (r) goto done;
         }
     }
 
      
     r = (*rock->proc)(NULL, rock->procrock);
 
  done:
     for (i = 0; i < rock->globs.count; i++) {
         glob *g = ptrarray_nth(&rock->globs, i);
         glob_free(&g);
     }
     ptrarray_fini(&rock->globs);
 
     return r;
 }