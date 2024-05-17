 expand_string_internal(uschar *string, BOOL ket_ends, uschar **left,
   BOOL skipping, BOOL honour_dollar, BOOL *resetok_p)
 {
 int ptr = 0;
 int size = Ustrlen(string)+ 64;
 int item_type;
 uschar *yield = store_get(size);
 uschar *s = string;
 uschar *save_expand_nstring[EXPAND_MAXN+1];
 int save_expand_nlength[EXPAND_MAXN+1];
 BOOL resetok = TRUE;
 
 expand_string_forcedfail = FALSE;
 expand_string_message = US"";
 
 while (*s != 0)
   {
   uschar *value;
   uschar name[256];
 
    
 
   if (*s == '\\')
     {
     if (s[1] == 0)
       {
       expand_string_message = US"\\ at end of string";
       goto EXPAND_FAILED;
       }
 
     if (s[1] == 'N')
       {
       uschar *t = s + 2;
       for (s = t; *s != 0; s++) if (*s == '\\' && s[1] == 'N') break;
       yield = string_cat(yield, &size, &ptr, t, s - t);
       if (*s != 0) s += 2;
       }
 
     else
       {
       uschar ch[1];
       ch[0] = string_interpret_escape(&s);
       s++;
       yield = string_cat(yield, &size, &ptr, ch, 1);
       }
 
     continue;
     }
 
    
    
 
    
   if (ket_ends && *s == '}') break;
 
   if (*s != '$' || !honour_dollar)
     {
     yield = string_cat(yield, &size, &ptr, s++, 1);
     continue;
     }
 
    	 
 
   if (isalpha((*(++s))))
     {
     int len;
     int newsize = 0;
 
     s = read_name(name, sizeof(name), s, US"_");
 
      
 
     if (ptr == 0 && yield != NULL)
       {
       if (resetok) store_reset(yield);
       yield = NULL;
       size = 0;
       }
 
      
 
     if (Ustrncmp(name, "h_", 2) == 0 ||
         Ustrncmp(name, "rh_", 3) == 0 ||
         Ustrncmp(name, "bh_", 3) == 0 ||
         Ustrncmp(name, "header_", 7) == 0 ||
         Ustrncmp(name, "rheader_", 8) == 0 ||
         Ustrncmp(name, "bheader_", 8) == 0)
       {
       BOOL want_raw = (name[0] == 'r')? TRUE : FALSE;
       uschar *charset = (name[0] == 'b')? NULL : headers_charset;
       s = read_header_name(name, sizeof(name), s);
       value = find_header(name, FALSE, &newsize, want_raw, charset);
 
        
 
       if (value == NULL)
         {
         if (Ustrchr(name, '}') != NULL) malformed_header = TRUE;
         continue;
         }
       }
 
      
 
     else
       {
       value = find_variable(name, FALSE, skipping, &newsize);
       if (value == NULL)
         {
         expand_string_message =
           string_sprintf("unknown variable name \"%s\"", name);
           check_variable_error_message(name);
         goto EXPAND_FAILED;
         }
       }
 
      
 
     len = Ustrlen(value);
     if (yield == NULL && newsize != 0)
       {
       yield = value;
       size = newsize;
       ptr = len;
       }
     else yield = string_cat(yield, &size, &ptr, value, len);
 
     continue;
     }
 
   if (isdigit(*s))
     {
     int n;
     s = read_number(&n, s);
     if (n >= 0 && n <= expand_nmax)
       yield = string_cat(yield, &size, &ptr, expand_nstring[n],
         expand_nlength[n]);
     continue;
     }
 
    		 
 
   if (*s != '{')							 
     {
     expand_string_message = US"$ not followed by letter, digit, or {";	 
     goto EXPAND_FAILED;
     }
 
    
 
   if (isdigit((*(++s))))
     {
     int n;
     s = read_number(&n, s);		 
     if (*s++ != '}')
       {					 
       expand_string_message = US"} expected after number";
       goto EXPAND_FAILED;
       }
     if (n >= 0 && n <= expand_nmax)
       yield = string_cat(yield, &size, &ptr, expand_nstring[n],
         expand_nlength[n]);
     continue;
     }
 
   if (!isalpha(*s))
     {
     expand_string_message = US"letter or digit expected after ${";	 
     goto EXPAND_FAILED;
     }
 
    
 
   s = read_name(name, sizeof(name), s, US"_-");
   item_type = chop_match(name, item_table, sizeof(item_table)/sizeof(uschar *));
 
   switch(item_type)
     {
      
 
     case EITEM_ACL:
        
       {
       uschar *sub[10];	 
       uschar *user_msg;
 
       switch(read_subs(sub, 10, 1, &s, skipping, TRUE, US"acl", &resetok))
         {
         case 1: goto EXPAND_FAILED_CURLY;
         case 2:
         case 3: goto EXPAND_FAILED;
         }
       if (skipping) continue;
 
       resetok = FALSE;
       switch(eval_acl(sub, sizeof(sub)/sizeof(*sub), &user_msg))
 	{
 	case OK:
 	case FAIL:
 	  DEBUG(D_expand)
 	    debug_printf("acl expansion yield: %s\n", user_msg);
 	  if (user_msg)
             yield = string_cat(yield, &size, &ptr, user_msg, Ustrlen(user_msg));
 	  continue;
 
 	case DEFER:
           expand_string_forcedfail = TRUE;
 	default:
           expand_string_message = string_sprintf("error from acl \"%s\"", sub[0]);
 	  goto EXPAND_FAILED;
 	}
       }
 
      
 
     case EITEM_IF:
       {
       BOOL cond = FALSE;
       uschar *next_s;
       int save_expand_nmax =
         save_expand_strings(save_expand_nstring, save_expand_nlength);
 
       while (isspace(*s)) s++;
       next_s = eval_condition(s, &resetok, skipping? NULL : &cond);
       if (next_s == NULL) goto EXPAND_FAILED;   
 
       DEBUG(D_expand)
         debug_printf("condition: %.*s\n   result: %s\n", (int)(next_s - s), s,
           cond? "true" : "false");
 
       s = next_s;
 
        
 
       switch(process_yesno(
                skipping,                      
                cond,                          
                lookup_value,                  
                &s,                            
                &yield,                        
                &size,                         
                &ptr,                          
                US"if",                        
 	       &resetok))
         {
         case 1: goto EXPAND_FAILED;           
         case 2: goto EXPAND_FAILED_CURLY;     
         }
 
        
 
       restore_expand_strings(save_expand_nmax, save_expand_nstring,
         save_expand_nlength);
       continue;
       }
 
      
 
     case EITEM_LOOKUP:
       {
       int stype, partial, affixlen, starflags;
       int expand_setup = 0;
       int nameptr = 0;
       uschar *key, *filename, *affix;
       uschar *save_lookup_value = lookup_value;
       int save_expand_nmax =
         save_expand_strings(save_expand_nstring, save_expand_nlength);
 
       if ((expand_forbid & RDO_LOOKUP) != 0)
         {
         expand_string_message = US"lookup expansions are not permitted";
         goto EXPAND_FAILED;
         }
 
        
 
       while (isspace(*s)) s++;
       if (*s == '{')					 
         {
         key = expand_string_internal(s+1, TRUE, &s, skipping, TRUE, &resetok);
         if (key == NULL) goto EXPAND_FAILED;		 
         if (*s++ != '}') goto EXPAND_FAILED_CURLY;
         while (isspace(*s)) s++;
         }
       else key = NULL;
 
        
 
       if (!isalpha(*s))
         {
         expand_string_message = US"missing lookup type";
         goto EXPAND_FAILED;
         }
 
        		 
 
       while (*s != 0 && *s != '{' && !isspace(*s))	 
         {
         if (nameptr < sizeof(name) - 1) name[nameptr++] = *s;
         s++;
         }
       name[nameptr] = 0;
       while (isspace(*s)) s++;
 
        
 
       stype = search_findtype_partial(name, &partial, &affix, &affixlen,
         &starflags);
       if (stype < 0)
         {
         expand_string_message = search_error_message;
         goto EXPAND_FAILED;
         }
 
        
 
       if (!mac_islookup(stype, lookup_querystyle|lookup_absfilequery))
         {
         if (key == NULL)
           {
           expand_string_message = string_sprintf("missing {key} for single-"
             "key \"%s\" lookup", name);
           goto EXPAND_FAILED;
           }
         }
       else
         {
         if (key != NULL)
           {
           expand_string_message = string_sprintf("a single key was given for "
             "lookup type \"%s\", which is not a single-key lookup type", name);
           goto EXPAND_FAILED;
           }
         }
 
        
 
       if (*s != '{') goto EXPAND_FAILED_CURLY;
       filename = expand_string_internal(s+1, TRUE, &s, skipping, TRUE, &resetok);
       if (filename == NULL) goto EXPAND_FAILED;
       if (*s++ != '}') goto EXPAND_FAILED_CURLY;
       while (isspace(*s)) s++;
 
        
 
       if (key == NULL)
         {
         while (isspace(*filename)) filename++;
         key = filename;
 
         if (mac_islookup(stype, lookup_querystyle))
           {
           filename = NULL;
           }
         else
           {
           if (*filename != '/')
             {
             expand_string_message = string_sprintf(
               "absolute file name expected for \"%s\" lookup", name);
             goto EXPAND_FAILED;
             }
           while (*key != 0 && !isspace(*key)) key++;
           if (*key != 0) *key++ = 0;
           }
         }
 
        
 
       if (skipping)
         lookup_value = NULL;
       else
         {
         void *handle = search_open(filename, stype, 0, NULL, NULL);
         if (handle == NULL)
           {
           expand_string_message = search_error_message;
           goto EXPAND_FAILED;
           }
         lookup_value = search_find(handle, filename, key, partial, affix,
           affixlen, starflags, &expand_setup);
         if (search_find_defer)
           {
           expand_string_message =
             string_sprintf("lookup of \"%s\" gave DEFER: %s",
               string_printing2(key, FALSE), search_error_message);
           goto EXPAND_FAILED;
           }
         if (expand_setup > 0) expand_nmax = expand_setup;
         }
 
        
 
       switch(process_yesno(
                skipping,                      
                lookup_value != NULL,          
                save_lookup_value,             
                &s,                            
                &yield,                        
                &size,                         
                &ptr,                          
                US"lookup",                    
 	       &resetok))
         {
         case 1: goto EXPAND_FAILED;           
         case 2: goto EXPAND_FAILED_CURLY;     
         }
 
        
 
       restore_expand_strings(save_expand_nmax, save_expand_nstring,
         save_expand_nlength);
       continue;
       }
 
      
 
     #define EXIM_PERL_MAX_ARGS 8
 
     case EITEM_PERL:
     #ifndef EXIM_PERL
     expand_string_message = US"\"${perl\" encountered, but this facility "	 
       "is not included in this binary";
     goto EXPAND_FAILED;
 
     #else    
       {
       uschar *sub_arg[EXIM_PERL_MAX_ARGS + 2];
       uschar *new_yield;
 
       if ((expand_forbid & RDO_PERL) != 0)
         {
         expand_string_message = US"Perl calls are not permitted";
         goto EXPAND_FAILED;
         }
 
       switch(read_subs(sub_arg, EXIM_PERL_MAX_ARGS + 1, 1, &s, skipping, TRUE,
            US"perl", &resetok))
         {
         case 1: goto EXPAND_FAILED_CURLY;
         case 2:
         case 3: goto EXPAND_FAILED;
         }
 
        
 
       if (skipping) continue;
 
        
 
       if (!opt_perl_started)
         {
         uschar *initerror;
         if (opt_perl_startup == NULL)
           {
           expand_string_message = US"A setting of perl_startup is needed when "
             "using the Perl interpreter";
           goto EXPAND_FAILED;
           }
         DEBUG(D_any) debug_printf("Starting Perl interpreter\n");
         initerror = init_perl(opt_perl_startup);
         if (initerror != NULL)
           {
           expand_string_message =
             string_sprintf("error in perl_startup code: %s\n", initerror);
           goto EXPAND_FAILED;
           }
         opt_perl_started = TRUE;
         }
 
        
 
       sub_arg[EXIM_PERL_MAX_ARGS + 1] = NULL;
       new_yield = call_perl_cat(yield, &size, &ptr, &expand_string_message,
         sub_arg[0], sub_arg + 1);
 
        
 
       if (new_yield == NULL)
         {
         if (expand_string_message == NULL)
           {
           expand_string_message =
             string_sprintf("Perl subroutine \"%s\" returned undef to force "
               "failure", sub_arg[0]);
           expand_string_forcedfail = TRUE;
           }
         goto EXPAND_FAILED;
         }
 
        
 
       expand_string_forcedfail = FALSE;
       yield = new_yield;
       continue;
       }
     #endif  
 
      
 
     case EITEM_PRVS:
       {
       uschar *sub_arg[3];
       uschar *p,*domain;
 
       switch(read_subs(sub_arg, 3, 2, &s, skipping, TRUE, US"prvs", &resetok))
         {
         case 1: goto EXPAND_FAILED_CURLY;
         case 2:
         case 3: goto EXPAND_FAILED;
         }
 
        
       if (skipping) continue;
 
        
       domain = Ustrrchr(sub_arg[0],'@');
       if ( (domain == NULL) || (domain == sub_arg[0]) || (Ustrlen(domain) == 1) )
         {
         expand_string_message = US"prvs first argument must be a qualified email address";
         goto EXPAND_FAILED;
         }
 
        
 
       if (sub_arg[2] != NULL &&
           (!isdigit(sub_arg[2][0]) || sub_arg[2][1] != 0))
         {
         expand_string_message = US"prvs second argument must be a single digit";
         goto EXPAND_FAILED;
         }
 
       p = prvs_hmac_sha1(sub_arg[0],sub_arg[1],sub_arg[2],prvs_daystamp(7));
       if (p == NULL)
         {
         expand_string_message = US"prvs hmac-sha1 conversion failed";
         goto EXPAND_FAILED;
         }
 
        
       *domain++ = '\0';
 
       yield = string_cat(yield,&size,&ptr,US"prvs=",5);
       string_cat(yield,&size,&ptr,(sub_arg[2] != NULL) ? sub_arg[2] : US"0", 1);
       string_cat(yield,&size,&ptr,prvs_daystamp(7),3);
       string_cat(yield,&size,&ptr,p,6);
       string_cat(yield,&size,&ptr,US"=",1);
       string_cat(yield,&size,&ptr,sub_arg[0],Ustrlen(sub_arg[0]));
       string_cat(yield,&size,&ptr,US"@",1);
       string_cat(yield,&size,&ptr,domain,Ustrlen(domain));
 
       continue;
       }
 
      
 
     case EITEM_PRVSCHECK:
       {
       uschar *sub_arg[3];
       int mysize = 0, myptr = 0;
       const pcre *re;
       uschar *p;
 
        
 
        
       prvscheck_result = NULL;
       prvscheck_address = NULL;
       prvscheck_keynum = NULL;
 
       switch(read_subs(sub_arg, 1, 1, &s, skipping, FALSE, US"prvs", &resetok))
         {
         case 1: goto EXPAND_FAILED_CURLY;
         case 2:
         case 3: goto EXPAND_FAILED;
         }
 
       re = regex_must_compile(US"^prvs\\=([0-9])([0-9]{3})([A-F0-9]{6})\\=(.+)\\@(.+)$",
                               TRUE,FALSE);
 
       if (regex_match_and_setup(re,sub_arg[0],0,-1))
         {
         uschar *local_part = string_copyn(expand_nstring[4],expand_nlength[4]);
         uschar *key_num = string_copyn(expand_nstring[1],expand_nlength[1]);
         uschar *daystamp = string_copyn(expand_nstring[2],expand_nlength[2]);
         uschar *hash = string_copyn(expand_nstring[3],expand_nlength[3]);
         uschar *domain = string_copyn(expand_nstring[5],expand_nlength[5]);
 
         DEBUG(D_expand) debug_printf("prvscheck localpart: %s\n", local_part);
         DEBUG(D_expand) debug_printf("prvscheck key number: %s\n", key_num);
         DEBUG(D_expand) debug_printf("prvscheck daystamp: %s\n", daystamp);
         DEBUG(D_expand) debug_printf("prvscheck hash: %s\n", hash);
         DEBUG(D_expand) debug_printf("prvscheck domain: %s\n", domain);
 
          
         prvscheck_address = string_cat(NULL, &mysize, &myptr, local_part, Ustrlen(local_part));
         string_cat(prvscheck_address,&mysize,&myptr,US"@",1);
         string_cat(prvscheck_address,&mysize,&myptr,domain,Ustrlen(domain));
         prvscheck_address[myptr] = '\0';
         prvscheck_keynum = string_copy(key_num);
 
          
         switch(read_subs(sub_arg, 1, 1, &s, skipping, FALSE, US"prvs", &resetok))
           {
           case 1: goto EXPAND_FAILED_CURLY;
           case 2:
           case 3: goto EXPAND_FAILED;
           }
 
          
 
         p = prvs_hmac_sha1(prvscheck_address, sub_arg[0], prvscheck_keynum,
           daystamp);
 
         if (p == NULL)
           {
           expand_string_message = US"hmac-sha1 conversion failed";
           goto EXPAND_FAILED;
           }
 
         DEBUG(D_expand) debug_printf("prvscheck: received hash is %s\n", hash);
         DEBUG(D_expand) debug_printf("prvscheck:      own hash is %s\n", p);
 
         if (Ustrcmp(p,hash) == 0)
           {
            
           uschar *now = prvs_daystamp(0);
           unsigned int inow = 0,iexpire = 1;
 
           (void)sscanf(CS now,"%u",&inow);
           (void)sscanf(CS daystamp,"%u",&iexpire);
 
            
           if ( (iexpire < 7) && (inow >= 993) ) inow = 0;
 
           if (iexpire >= inow)
             {
             prvscheck_result = US"1";
             DEBUG(D_expand) debug_printf("prvscheck: success, $pvrs_result set to 1\n");
             }
             else
             {
             prvscheck_result = NULL;
             DEBUG(D_expand) debug_printf("prvscheck: signature expired, $pvrs_result unset\n");
             }
           }
         else
           {
           prvscheck_result = NULL;
           DEBUG(D_expand) debug_printf("prvscheck: hash failure, $pvrs_result unset\n");
           }
 
          
 
         switch(read_subs(sub_arg, 1, 0, &s, skipping, TRUE, US"prvs", &resetok))
           {
           case 1: goto EXPAND_FAILED_CURLY;
           case 2:
           case 3: goto EXPAND_FAILED;
           }
 
         if (sub_arg[0] == NULL || *sub_arg[0] == '\0')
           yield = string_cat(yield,&size,&ptr,prvscheck_address,Ustrlen(prvscheck_address));
         else
           yield = string_cat(yield,&size,&ptr,sub_arg[0],Ustrlen(sub_arg[0]));
 
          
 
         prvscheck_address = NULL;
         prvscheck_keynum = NULL;
         }
       else
         {
          
 
         switch(read_subs(sub_arg, 2, 1, &s, skipping, TRUE, US"prvs", &resetok))
           {
           case 1: goto EXPAND_FAILED_CURLY;
           case 2:
           case 3: goto EXPAND_FAILED;
           }
         }
 
       continue;
       }
 
      
 
     case EITEM_READFILE:
       {
       FILE *f;
       uschar *sub_arg[2];
 
       if ((expand_forbid & RDO_READFILE) != 0)
         {
         expand_string_message = US"file insertions are not permitted";
         goto EXPAND_FAILED;
         }
 
       switch(read_subs(sub_arg, 2, 1, &s, skipping, TRUE, US"readfile", &resetok))
         {
         case 1: goto EXPAND_FAILED_CURLY;
         case 2:
         case 3: goto EXPAND_FAILED;
         }
 
        
 
       if (skipping) continue;
 
        
 
       f = Ufopen(sub_arg[0], "rb");
       if (f == NULL)
         {
         expand_string_message = string_open_failed(errno, "%s", sub_arg[0]);
         goto EXPAND_FAILED;
         }
 
       yield = cat_file(f, yield, &size, &ptr, sub_arg[1]);
       (void)fclose(f);
       continue;
       }
 
      
 
     case EITEM_READSOCK:
       {
       int fd;
       int timeout = 5;
       int save_ptr = ptr;
       FILE *f;
       struct sockaddr_un sockun;          
       uschar *arg;
       uschar *sub_arg[4];
 
       if ((expand_forbid & RDO_READSOCK) != 0)
         {
         expand_string_message = US"socket insertions are not permitted";
         goto EXPAND_FAILED;
         }
 
        
 
       switch(read_subs(sub_arg, 4, 2, &s, skipping, FALSE, US"readsocket", &resetok))
         {
         case 1: goto EXPAND_FAILED_CURLY;
         case 2:                              
         case 3: goto EXPAND_FAILED;
         }
 
        
 
       if (sub_arg[2] != NULL)
         {
         timeout = readconf_readtime(sub_arg[2], 0, FALSE);
         if (timeout < 0)
           {
           expand_string_message = string_sprintf("bad time value %s",
             sub_arg[2]);
           goto EXPAND_FAILED;
           }
         }
       else sub_arg[3] = NULL;                      
 
        
 
       if (!skipping)
         {
          
 
         if (Ustrncmp(sub_arg[0], "inet:", 5) == 0)
           {
           int port;
           uschar *server_name = sub_arg[0] + 5;
           uschar *port_name = Ustrrchr(server_name, ':');
 
            
 
           if (port_name == NULL)
             {
             expand_string_message =
               string_sprintf("missing port for readsocket %s", sub_arg[0]);
             goto EXPAND_FAILED;
             }
           *port_name++ = 0;            
 
           if (isdigit(*port_name))
             {
             uschar *end;
             port = Ustrtol(port_name, &end, 0);
             if (end != port_name + Ustrlen(port_name))
               {
               expand_string_message =
                 string_sprintf("invalid port number %s", port_name);
               goto EXPAND_FAILED;
               }
             }
           else
             {
             struct servent *service_info = getservbyname(CS port_name, "tcp");
             if (service_info == NULL)
               {
               expand_string_message = string_sprintf("unknown port \"%s\"",
                 port_name);
               goto EXPAND_FAILED;
               }
             port = ntohs(service_info->s_port);
             }
 
 	  if ((fd = ip_connectedsocket(SOCK_STREAM, server_name, port, port,
 		  timeout, NULL, &expand_string_message)) < 0)
               goto SOCK_FAIL;
           }
 
          
 
         else
           {
           int rc;
           if ((fd = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
             {
             expand_string_message = string_sprintf("failed to create socket: %s",
               strerror(errno));
             goto SOCK_FAIL;
             }
 
           sockun.sun_family = AF_UNIX;
           sprintf(sockun.sun_path, "%.*s", (int)(sizeof(sockun.sun_path)-1),
             sub_arg[0]);
 
           sigalrm_seen = FALSE;
           alarm(timeout);
           rc = connect(fd, (struct sockaddr *)(&sockun), sizeof(sockun));
           alarm(0);
           if (sigalrm_seen)
             {
             expand_string_message = US "socket connect timed out";
             goto SOCK_FAIL;
             }
           if (rc < 0)
             {
             expand_string_message = string_sprintf("failed to connect to socket "
               "%s: %s", sub_arg[0], strerror(errno));
             goto SOCK_FAIL;
             }
           }
 
         DEBUG(D_expand) debug_printf("connected to socket %s\n", sub_arg[0]);
 
 	 
 	if (running_in_test_harness) millisleep(100);
 
          
 
         if (sub_arg[1][0] != 0)
           {
           int len = Ustrlen(sub_arg[1]);
           DEBUG(D_expand) debug_printf("writing \"%s\" to socket\n",
             sub_arg[1]);
           if (write(fd, sub_arg[1], len) != len)
             {
             expand_string_message = string_sprintf("request write to socket "
               "failed: %s", strerror(errno));
             goto SOCK_FAIL;
             }
           }
 
          
 
         #ifdef SHUT_WR
         shutdown(fd, SHUT_WR);
         #endif
 
 	if (running_in_test_harness) millisleep(100);
 
          
 
         f = fdopen(fd, "rb");
         sigalrm_seen = FALSE;
         alarm(timeout);
         yield = cat_file(f, yield, &size, &ptr, sub_arg[3]);
         alarm(0);
         (void)fclose(f);
 
          
 
         if (sigalrm_seen)
           {
           ptr = save_ptr;
           expand_string_message = US "socket read timed out";
           goto SOCK_FAIL;
           }
         }
 
        
 
       if (*s == '{')
         {
         if (expand_string_internal(s+1, TRUE, &s, TRUE, TRUE, &resetok) == NULL)
           goto EXPAND_FAILED;
         if (*s++ != '}') goto EXPAND_FAILED_CURLY;
         while (isspace(*s)) s++;
         }
       if (*s++ != '}') goto EXPAND_FAILED_CURLY;
       continue;
 
        
 
       SOCK_FAIL:
       if (*s != '{') goto EXPAND_FAILED;
       DEBUG(D_any) debug_printf("%s\n", expand_string_message);
       arg = expand_string_internal(s+1, TRUE, &s, FALSE, TRUE, &resetok);
       if (arg == NULL) goto EXPAND_FAILED;
       yield = string_cat(yield, &size, &ptr, arg, Ustrlen(arg));
       if (*s++ != '}') goto EXPAND_FAILED_CURLY;
       while (isspace(*s)) s++;
       if (*s++ != '}') goto EXPAND_FAILED_CURLY;
       continue;
       }
 
      
 
     case EITEM_RUN:
       {
       FILE *f;
       uschar *arg;
       uschar **argv;
       pid_t pid;
       int fd_in, fd_out;
       int lsize = 0;
       int lptr = 0;
 
       if ((expand_forbid & RDO_RUN) != 0)
         {
         expand_string_message = US"running a command is not permitted";
         goto EXPAND_FAILED;
         }
 
       while (isspace(*s)) s++;
       if (*s != '{') goto EXPAND_FAILED_CURLY;
       arg = expand_string_internal(s+1, TRUE, &s, skipping, TRUE, &resetok);
       if (arg == NULL) goto EXPAND_FAILED;
       while (isspace(*s)) s++;
       if (*s++ != '}') goto EXPAND_FAILED_CURLY;
 
       if (skipping)    
         {
         runrc = 0;
         }
       else
         {
         if (!transport_set_up_command(&argv,     
             arg,                                 
             FALSE,                               
             0,                                   
             NULL,                                
             US"${run} expansion",                
             &expand_string_message))             
           {
           goto EXPAND_FAILED;
           }
 
          
 
         pid = child_open(argv, NULL, 0077, &fd_in, &fd_out, TRUE);
 
         if (pid < 0)
           {
           expand_string_message =
             string_sprintf("couldn't create child process: %s", strerror(errno));
           goto EXPAND_FAILED;
           }
 
          
 
         (void)close(fd_in);
 
          
 
         f = fdopen(fd_out, "rb");
         sigalrm_seen = FALSE;
         alarm(60);
         lookup_value = cat_file(f, lookup_value, &lsize, &lptr, NULL);
         alarm(0);
         (void)fclose(f);
 
          
 
         if (sigalrm_seen == TRUE || (runrc = child_close(pid, 30)) < 0)
           {
           if (sigalrm_seen == TRUE || runrc == -256)
             {
             expand_string_message = string_sprintf("command timed out");
             killpg(pid, SIGKILL);        
             }
 
           else if (runrc == -257)
             expand_string_message = string_sprintf("wait() failed: %s",
               strerror(errno));
 
           else
             expand_string_message = string_sprintf("command killed by signal %d",
               -runrc);
 
           goto EXPAND_FAILED;
           }
         }
 
        
 
       switch(process_yesno(
                skipping,                      
                runrc == 0,                    
                lookup_value,                  
                &s,                            
                &yield,                        
                &size,                         
                &ptr,                          
                US"run",                       
 	       &resetok))
         {
         case 1: goto EXPAND_FAILED;           
         case 2: goto EXPAND_FAILED_CURLY;     
         }
 
       continue;
       }
 
      
 
     case EITEM_TR:
       {
       int oldptr = ptr;
       int o2m;
       uschar *sub[3];
 
       switch(read_subs(sub, 3, 3, &s, skipping, TRUE, US"tr", &resetok))
         {
         case 1: goto EXPAND_FAILED_CURLY;
         case 2:
         case 3: goto EXPAND_FAILED;
         }
 
       yield = string_cat(yield, &size, &ptr, sub[0], Ustrlen(sub[0]));
       o2m = Ustrlen(sub[2]) - 1;
 
       if (o2m >= 0) for (; oldptr < ptr; oldptr++)
         {
         uschar *m = Ustrrchr(sub[1], yield[oldptr]);
         if (m != NULL)
           {
           int o = m - sub[1];
           yield[oldptr] = sub[2][(o < o2m)? o : o2m];
           }
         }
 
       continue;
       }
 
      
 
     case EITEM_HASH:
     case EITEM_LENGTH:
     case EITEM_NHASH:
     case EITEM_SUBSTR:
       {
       int i;
       int len;
       uschar *ret;
       int val[2] = { 0, -1 };
       uschar *sub[3];
 
        
 
       sub[2] = NULL;
       switch(read_subs(sub, (item_type == EITEM_LENGTH)? 2:3, 2, &s, skipping,
              TRUE, name, &resetok))
         {
         case 1: goto EXPAND_FAILED_CURLY;
         case 2:
         case 3: goto EXPAND_FAILED;
         }
 
        
 
       if (sub[2] == NULL)
         {
         sub[2] = sub[1];
         sub[1] = NULL;
         if (item_type == EITEM_LENGTH)
           {
           sub[1] = sub[0];
           sub[0] = NULL;
           }
         }
 
       for (i = 0; i < 2; i++)
         {
         if (sub[i] == NULL) continue;
         val[i] = (int)Ustrtol(sub[i], &ret, 10);
         if (*ret != 0 || (i != 0 && val[i] < 0))
           {
           expand_string_message = string_sprintf("\"%s\" is not a%s number "
             "(in \"%s\" expansion)", sub[i], (i != 0)? " positive" : "", name);
           goto EXPAND_FAILED;
           }
         }
 
       ret =
         (item_type == EITEM_HASH)?
           compute_hash(sub[2], val[0], val[1], &len) :
         (item_type == EITEM_NHASH)?
           compute_nhash(sub[2], val[0], val[1], &len) :
           extract_substr(sub[2], val[0], val[1], &len);
 
       if (ret == NULL) goto EXPAND_FAILED;
       yield = string_cat(yield, &size, &ptr, ret, len);
       continue;
       }
 
      
 
     #define MAX_HASHLEN      20
     #define MAX_HASHBLOCKLEN 64
 
     case EITEM_HMAC:
       {
       uschar *sub[3];
       md5 md5_base;
       sha1 sha1_base;
       void *use_base;
       int type, i;
       int hashlen;       
       int hashblocklen;  
       uschar *keyptr, *p;
       unsigned int keylen;
 
       uschar keyhash[MAX_HASHLEN];
       uschar innerhash[MAX_HASHLEN];
       uschar finalhash[MAX_HASHLEN];
       uschar finalhash_hex[2*MAX_HASHLEN];
       uschar innerkey[MAX_HASHBLOCKLEN];
       uschar outerkey[MAX_HASHBLOCKLEN];
 
       switch (read_subs(sub, 3, 3, &s, skipping, TRUE, name, &resetok))
         {
         case 1: goto EXPAND_FAILED_CURLY;
         case 2:
         case 3: goto EXPAND_FAILED;
         }
 
       if (Ustrcmp(sub[0], "md5") == 0)
         {
         type = HMAC_MD5;
         use_base = &md5_base;
         hashlen = 16;
         hashblocklen = 64;
         }
       else if (Ustrcmp(sub[0], "sha1") == 0)
         {
         type = HMAC_SHA1;
         use_base = &sha1_base;
         hashlen = 20;
         hashblocklen = 64;
         }
       else
         {
         expand_string_message =
           string_sprintf("hmac algorithm \"%s\" is not recognised", sub[0]);
         goto EXPAND_FAILED;
         }
 
       keyptr = sub[1];
       keylen = Ustrlen(keyptr);
 
        
 
       if (keylen > hashblocklen)
         {
         chash_start(type, use_base);
         chash_end(type, use_base, keyptr, keylen, keyhash);
         keyptr = keyhash;
         keylen = hashlen;
         }
 
        
 
       memset(innerkey, 0x36, hashblocklen);
       memset(outerkey, 0x5c, hashblocklen);
 
       for (i = 0; i < keylen; i++)
         {
         innerkey[i] ^= keyptr[i];
         outerkey[i] ^= keyptr[i];
         }
 
        
 
       chash_start(type, use_base);
       chash_mid(type, use_base, innerkey);
       chash_end(type, use_base, sub[2], Ustrlen(sub[2]), innerhash);
 
       chash_start(type, use_base);
       chash_mid(type, use_base, outerkey);
       chash_end(type, use_base, innerhash, hashlen, finalhash);
 
        
 
       p = finalhash_hex;
       for (i = 0; i < hashlen; i++)
         {
         *p++ = hex_digits[(finalhash[i] & 0xf0) >> 4];
         *p++ = hex_digits[finalhash[i] & 0x0f];
         }
 
       DEBUG(D_any) debug_printf("HMAC[%s](%.*s,%.*s)=%.*s\n", sub[0],
         (int)keylen, keyptr, Ustrlen(sub[2]), sub[2], hashlen*2, finalhash_hex);
 
       yield = string_cat(yield, &size, &ptr, finalhash_hex, hashlen*2);
       }
 
     continue;
 
      
 
     case EITEM_SG:
       {
       const pcre *re;
       int moffset, moffsetextra, slen;
       int roffset;
       int emptyopt;
       const uschar *rerror;
       uschar *subject;
       uschar *sub[3];
       int save_expand_nmax =
         save_expand_strings(save_expand_nstring, save_expand_nlength);
 
       switch(read_subs(sub, 3, 3, &s, skipping, TRUE, US"sg", &resetok))
         {
         case 1: goto EXPAND_FAILED_CURLY;
         case 2:
         case 3: goto EXPAND_FAILED;
         }
 
        
 
       re = pcre_compile(CS sub[1], PCRE_COPT, (const char **)&rerror, &roffset,
         NULL);
 
       if (re == NULL)
         {
         expand_string_message = string_sprintf("regular expression error in "
           "\"%s\": %s at offset %d", sub[1], rerror, roffset);
         goto EXPAND_FAILED;
         }
 
        
 
       subject = sub[0];
       slen = Ustrlen(sub[0]);
       moffset = moffsetextra = 0;
       emptyopt = 0;
 
       for (;;)
         {
         int ovector[3*(EXPAND_MAXN+1)];
         int n = pcre_exec(re, NULL, CS subject, slen, moffset + moffsetextra,
           PCRE_EOPT | emptyopt, ovector, sizeof(ovector)/sizeof(int));
         int nn;
         uschar *insert;
 
          
 
         if (n < 0)
           {
           if (emptyopt != 0)
             {
             moffsetextra = 1;
             emptyopt = 0;
             continue;
             }
           yield = string_cat(yield, &size, &ptr, subject+moffset, slen-moffset);
           break;
           }
 
          
 
         if (n == 0) n = EXPAND_MAXN + 1;
         expand_nmax = 0;
         for (nn = 0; nn < n*2; nn += 2)
           {
           expand_nstring[expand_nmax] = subject + ovector[nn];
           expand_nlength[expand_nmax++] = ovector[nn+1] - ovector[nn];
           }
         expand_nmax--;
 
          
 
         yield = string_cat(yield, &size, &ptr, subject + moffset,
           ovector[0] - moffset);
         insert = expand_string(sub[2]);
         if (insert == NULL) goto EXPAND_FAILED;
         yield = string_cat(yield, &size, &ptr, insert, Ustrlen(insert));
 
         moffset = ovector[1];
         moffsetextra = 0;
         emptyopt = 0;
 
          
 
         if (ovector[0] == ovector[1])
           {
           if (ovector[0] == slen) break;
           emptyopt = PCRE_NOTEMPTY | PCRE_ANCHORED;
           }
         }
 
        
 
       restore_expand_strings(save_expand_nmax, save_expand_nstring,
         save_expand_nlength);
       continue;
       }
 
      
 
     case EITEM_EXTRACT:
       {
       int i;
       int j = 2;
       int field_number = 1;
       BOOL field_number_set = FALSE;
       uschar *save_lookup_value = lookup_value;
       uschar *sub[3];
       int save_expand_nmax =
         save_expand_strings(save_expand_nstring, save_expand_nlength);
 
        
 
       for (i = 0; i < j; i++)
         {
         while (isspace(*s)) s++;
         if (*s == '{') 						 
           {
           sub[i] = expand_string_internal(s+1, TRUE, &s, skipping, TRUE, &resetok);
           if (sub[i] == NULL) goto EXPAND_FAILED;		 
           if (*s++ != '}') goto EXPAND_FAILED_CURLY;
 
            
 
           if (i == 0)
             {
             int len;
             int x = 0;
             uschar *p = sub[0];
 
             while (isspace(*p)) p++;
             sub[0] = p;
 
             len = Ustrlen(p);
             while (len > 0 && isspace(p[len-1])) len--;
             p[len] = 0;
 
             if (*p == 0 && !skipping)
               {
               expand_string_message = US"first argument of \"extract\" must "
                 "not be empty";
               goto EXPAND_FAILED;
               }
 
             if (*p == '-')
               {
               field_number = -1;
               p++;
               }
             while (*p != 0 && isdigit(*p)) x = x * 10 + *p++ - '0';
             if (*p == 0)
               {
               field_number *= x;
               j = 3;                
               field_number_set = TRUE;
               }
             }
           }
         else goto EXPAND_FAILED_CURLY;
         }
 
        
 
       lookup_value = skipping? NULL : field_number_set?
         expand_gettokened(field_number, sub[1], sub[2]) :
         expand_getkeyed(sub[0], sub[1]);
 
        
 
       switch(process_yesno(
                skipping,                      
                lookup_value != NULL,          
                save_lookup_value,             
                &s,                            
                &yield,                        
                &size,                         
                &ptr,                          
                US"extract",                   
 	       &resetok))
         {
         case 1: goto EXPAND_FAILED;           
         case 2: goto EXPAND_FAILED_CURLY;     
         }
 
        
 
       restore_expand_strings(save_expand_nmax, save_expand_nstring,
         save_expand_nlength);
 
       continue;
       }
 
      
 
     case EITEM_LISTEXTRACT:
       {
       int i;
       int field_number = 1;
       uschar *save_lookup_value = lookup_value;
       uschar *sub[2];
       int save_expand_nmax =
         save_expand_strings(save_expand_nstring, save_expand_nlength);
 
        
 
       for (i = 0; i < 2; i++)
         {
         while (isspace(*s)) s++;
         if (*s != '{')					 
 	  goto EXPAND_FAILED_CURLY;
 
 	sub[i] = expand_string_internal(s+1, TRUE, &s, skipping, TRUE, &resetok);
 	if (!sub[i])     goto EXPAND_FAILED;		 
 	if (*s++ != '}') goto EXPAND_FAILED_CURLY;
 
 	 
 
 	if (i == 0)
 	  {
 	  int len;
 	  int x = 0;
 	  uschar *p = sub[0];
 
 	  while (isspace(*p)) p++;
 	  sub[0] = p;
 
 	  len = Ustrlen(p);
 	  while (len > 0 && isspace(p[len-1])) len--;
 	  p[len] = 0;
 
 	  if (!*p && !skipping)
 	    {
 	    expand_string_message = US"first argument of \"listextract\" must "
 	      "not be empty";
 	    goto EXPAND_FAILED;
 	    }
 
 	  if (*p == '-')
 	    {
 	    field_number = -1;
 	    p++;
 	    }
 	  while (*p && isdigit(*p)) x = x * 10 + *p++ - '0';
 	  if (*p)
 	    {
 	    expand_string_message = US"first argument of \"listextract\" must "
 	      "be numeric";
 	    goto EXPAND_FAILED;
 	    }
 	  field_number *= x;
 	  }
         }
 
        
 
       lookup_value = skipping? NULL : expand_getlistele(field_number, sub[1]);
 
        
 
       switch(process_yesno(
                skipping,                      
                lookup_value != NULL,          
                save_lookup_value,             
                &s,                            
                &yield,                        
                &size,                         
                &ptr,                          
                US"extract",                   
 	       &resetok))
         {
         case 1: goto EXPAND_FAILED;           
         case 2: goto EXPAND_FAILED_CURLY;     
         }
 
        
 
       restore_expand_strings(save_expand_nmax, save_expand_nstring,
         save_expand_nlength);
 
       continue;
       }
 
 #ifdef SUPPORT_TLS
     case EITEM_CERTEXTRACT:
       {
       uschar *save_lookup_value = lookup_value;
       uschar *sub[2];
       int save_expand_nmax =
         save_expand_strings(save_expand_nstring, save_expand_nlength);
 
        
       while (isspace(*s)) s++;
       if (*s != '{')					 
 	goto EXPAND_FAILED_CURLY;
       sub[0] = expand_string_internal(s+1, TRUE, &s, skipping, TRUE, &resetok);
       if (!sub[0])     goto EXPAND_FAILED;		 
       if (*s++ != '}') goto EXPAND_FAILED_CURLY;
        
       {
       int len;
       uschar *p = sub[0];
 
       while (isspace(*p)) p++;
       sub[0] = p;
 
       len = Ustrlen(p);
       while (len > 0 && isspace(p[len-1])) len--;
       p[len] = 0;
       }
 
        
       while (isspace(*s)) s++;
       if (*s != '{')					 
 	goto EXPAND_FAILED_CURLY;
       if (*++s != '$')
         {
 	expand_string_message = US"second argument of \"certextract\" must "
 	  "be a certificate variable";
 	goto EXPAND_FAILED;
 	}
       sub[1] = expand_string_internal(s+1, TRUE, &s, skipping, FALSE, &resetok);
       if (!sub[1])     goto EXPAND_FAILED;		 
       if (*s++ != '}') goto EXPAND_FAILED_CURLY;
 
       if (skipping)
 	lookup_value = NULL;
       else
 	{
 	lookup_value = expand_getcertele(sub[0], sub[1]);
 	if (*expand_string_message) goto EXPAND_FAILED;
 	}
       switch(process_yesno(
                skipping,                      
                lookup_value != NULL,          
                save_lookup_value,             
                &s,                            
                &yield,                        
                &size,                         
                &ptr,                          
                US"extract",                   
 	       &resetok))
         {
         case 1: goto EXPAND_FAILED;           
         case 2: goto EXPAND_FAILED_CURLY;     
         }
 
       restore_expand_strings(save_expand_nmax, save_expand_nstring,
         save_expand_nlength);
       continue;
       }
 #endif	 
 
      
 
     case EITEM_FILTER:
     case EITEM_MAP:
     case EITEM_REDUCE:
       {
       int sep = 0;
       int save_ptr = ptr;
       uschar outsep[2] = { '\0', '\0' };
       uschar *list, *expr, *temp;
       uschar *save_iterate_item = iterate_item;
       uschar *save_lookup_value = lookup_value;
 
       while (isspace(*s)) s++;
       if (*s++ != '{') goto EXPAND_FAILED_CURLY;
 
       list = expand_string_internal(s, TRUE, &s, skipping, TRUE, &resetok);
       if (list == NULL) goto EXPAND_FAILED;
       if (*s++ != '}') goto EXPAND_FAILED_CURLY;
 
       if (item_type == EITEM_REDUCE)
         {
         while (isspace(*s)) s++;
         if (*s++ != '{') goto EXPAND_FAILED_CURLY;
         temp = expand_string_internal(s, TRUE, &s, skipping, TRUE, &resetok);
         if (temp == NULL) goto EXPAND_FAILED;
         lookup_value = temp;
         if (*s++ != '}') goto EXPAND_FAILED_CURLY;
         }
 
       while (isspace(*s)) s++;
       if (*s++ != '{') goto EXPAND_FAILED_CURLY;
 
       expr = s;
 
        
 
       if (item_type == EITEM_FILTER)
         {
         temp = eval_condition(expr, &resetok, NULL);
         if (temp != NULL) s = temp;
         }
       else
         {
         temp = expand_string_internal(s, TRUE, &s, TRUE, TRUE, &resetok);
         }
 
       if (temp == NULL)
         {
         expand_string_message = string_sprintf("%s inside \"%s\" item",
           expand_string_message, name);
         goto EXPAND_FAILED;
         }
 
       while (isspace(*s)) s++;
       if (*s++ != '}')
         {						 
         expand_string_message = string_sprintf("missing } at end of condition "
           "or expression inside \"%s\"", name);
         goto EXPAND_FAILED;
         }
 
       while (isspace(*s)) s++;				 
       if (*s++ != '}')
         {						 
         expand_string_message = string_sprintf("missing } at end of \"%s\"",
           name);
         goto EXPAND_FAILED;
         }
 
        
 
       if (skipping) continue;
       while ((iterate_item = string_nextinlist(&list, &sep, NULL, 0)) != NULL)
         {
         *outsep = (uschar)sep;       
 
         DEBUG(D_expand) debug_printf("%s: $item = \"%s\"\n", name, iterate_item);
 
         if (item_type == EITEM_FILTER)
           {
           BOOL condresult;
           if (eval_condition(expr, &resetok, &condresult) == NULL)
             {
             iterate_item = save_iterate_item;
             lookup_value = save_lookup_value;
             expand_string_message = string_sprintf("%s inside \"%s\" condition",
               expand_string_message, name);
             goto EXPAND_FAILED;
             }
           DEBUG(D_expand) debug_printf("%s: condition is %s\n", name,
             condresult? "true":"false");
           if (condresult)
             temp = iterate_item;     
           else
             continue;                
           }
 
          
 
         else
           {
           temp = expand_string_internal(expr, TRUE, NULL, skipping, TRUE, &resetok);
           if (temp == NULL)
             {
             iterate_item = save_iterate_item;
             expand_string_message = string_sprintf("%s inside \"%s\" item",
               expand_string_message, name);
             goto EXPAND_FAILED;
             }
           if (item_type == EITEM_REDUCE)
             {
             lookup_value = temp;       
             continue;                  
             }
           }
 
          
 
         if (ptr != save_ptr && (temp[0] == *outsep || temp[0] == 0))
           yield = string_cat(yield, &size, &ptr, US" ", 1);
 
          
 
         for (;;)
           {
           size_t seglen = Ustrcspn(temp, outsep);
             yield = string_cat(yield, &size, &ptr, temp, seglen + 1);
 
            
 
           if (temp[seglen] == '\0') { ptr--; break; }
           yield = string_cat(yield, &size, &ptr, outsep, 1);
           temp += seglen + 1;
           }
 
          
 
         yield = string_cat(yield, &size, &ptr, outsep, 1);
         }    
 
        
 
       if (item_type == EITEM_REDUCE)
         {
         yield = string_cat(yield, &size, &ptr, lookup_value,
           Ustrlen(lookup_value));
         lookup_value = save_lookup_value;   
         }
 
        
 
       else if (ptr != save_ptr) ptr--;
 
        
 
       iterate_item = save_iterate_item;
       continue;
       }
 
 
      
 
     #define EXPAND_DLFUNC_MAX_ARGS 8
 
     case EITEM_DLFUNC:
     #ifndef EXPAND_DLFUNC
     expand_string_message = US"\"${dlfunc\" encountered, but this facility "	 
       "is not included in this binary";
     goto EXPAND_FAILED;
 
     #else    
       {
       tree_node *t;
       exim_dlfunc_t *func;
       uschar *result;
       int status, argc;
       uschar *argv[EXPAND_DLFUNC_MAX_ARGS + 3];
 
       if ((expand_forbid & RDO_DLFUNC) != 0)
         {
         expand_string_message =
           US"dynamically-loaded functions are not permitted";
         goto EXPAND_FAILED;
         }
 
       switch(read_subs(argv, EXPAND_DLFUNC_MAX_ARGS + 2, 2, &s, skipping,
            TRUE, US"dlfunc", &resetok))
         {
         case 1: goto EXPAND_FAILED_CURLY;
         case 2:
         case 3: goto EXPAND_FAILED;
         }
 
        
 
       if (skipping) continue;
 
        
 
       t = tree_search(dlobj_anchor, argv[0]);
       if (t == NULL)
         {
         void *handle = dlopen(CS argv[0], RTLD_LAZY);
         if (handle == NULL)
           {
           expand_string_message = string_sprintf("dlopen \"%s\" failed: %s",
             argv[0], dlerror());
           log_write(0, LOG_MAIN|LOG_PANIC, "%s", expand_string_message);
           goto EXPAND_FAILED;
           }
         t = store_get_perm(sizeof(tree_node) + Ustrlen(argv[0]));
         Ustrcpy(t->name, argv[0]);
         t->data.ptr = handle;
         (void)tree_insertnode(&dlobj_anchor, t);
         }
 
        
 
       func = (exim_dlfunc_t *)dlsym(t->data.ptr, CS argv[1]);
       if (func == NULL)
         {
         expand_string_message = string_sprintf("dlsym \"%s\" in \"%s\" failed: "
           "%s", argv[1], argv[0], dlerror());
         log_write(0, LOG_MAIN|LOG_PANIC, "%s", expand_string_message);
         goto EXPAND_FAILED;
         }
 
        
 
       resetok = FALSE;
       result = NULL;
       for (argc = 0; argv[argc] != NULL; argc++);
       status = func(&result, argc - 2, &argv[2]);
       if(status == OK)
         {
         if (result == NULL) result = US"";
         yield = string_cat(yield, &size, &ptr, result, Ustrlen(result));
         continue;
         }
       else
         {
         expand_string_message = result == NULL ? US"(no message)" : result;
         if(status == FAIL_FORCED) expand_string_forcedfail = TRUE;
           else if(status != FAIL)
             log_write(0, LOG_MAIN|LOG_PANIC, "dlfunc{%s}{%s} failed (%d): %s",
               argv[0], argv[1], status, expand_string_message);
         goto EXPAND_FAILED;
         }
       }
     #endif  
     }	 
 
    
 
   if (*s == ':')
     {
     int c;
     uschar *arg = NULL;
     uschar *sub;
     var_entry *vp = NULL;
 
      
 
     if ((c = chop_match(name, op_table_underscore,
 	sizeof(op_table_underscore)/sizeof(uschar *))) < 0)
       {
       arg = Ustrchr(name, '_');
       if (arg != NULL) *arg = 0;
       c = chop_match(name, op_table_main,
         sizeof(op_table_main)/sizeof(uschar *));
       if (c >= 0) c += sizeof(op_table_underscore)/sizeof(uschar *);
       if (arg != NULL) *arg++ = '_';    
       }
 
      
     switch(c)
       {
 #ifdef SUPPORT_TLS
       case EOP_MD5:
       case EOP_SHA1:
       case EOP_SHA256:
 	if (s[1] == '$')
 	  {
 	  uschar * s1 = s;
 	  sub = expand_string_internal(s+2, TRUE, &s1, skipping,
 		  FALSE, &resetok);
 	  if (!sub)       goto EXPAND_FAILED;		 
 	  if (*s1 != '}') goto EXPAND_FAILED_CURLY;
 	  if ((vp = find_var_ent(sub)) && vp->type == vtype_cert)
 	    {
 	    s = s1+1;
 	    break;
 	    }
 	  vp = NULL;
 	  }
          
 #endif
       default:
 	sub = expand_string_internal(s+1, TRUE, &s, skipping, TRUE, &resetok);
 	if (!sub) goto EXPAND_FAILED;
 	s++;
 	break;
       }
 
      
 
     if (skipping && c >= 0) continue;
 
      
 
     switch(c)
       {
       case EOP_BASE62:
         {
         uschar *t;
         unsigned long int n = Ustrtoul(sub, &t, 10);
         if (*t != 0)
           {
           expand_string_message = string_sprintf("argument for base62 "
             "operator is \"%s\", which is not a decimal number", sub);
           goto EXPAND_FAILED;
           }
         t = string_base62(n);
         yield = string_cat(yield, &size, &ptr, t, Ustrlen(t));
         continue;
         }
 
        
 
       case EOP_BASE62D:
         {
         uschar buf[16];
         uschar *tt = sub;
         unsigned long int n = 0;
         while (*tt != 0)
           {
           uschar *t = Ustrchr(base62_chars, *tt++);
           if (t == NULL)
             {
             expand_string_message = string_sprintf("argument for base62d "
               "operator is \"%s\", which is not a base %d number", sub,
               BASE_62);
             goto EXPAND_FAILED;
             }
           n = n * BASE_62 + (t - base62_chars);
           }
         (void)sprintf(CS buf, "%ld", n);
         yield = string_cat(yield, &size, &ptr, buf, Ustrlen(buf));
         continue;
         }
 
       case EOP_EXPAND:
         {
         uschar *expanded = expand_string_internal(sub, FALSE, NULL, skipping, TRUE, &resetok);
         if (expanded == NULL)
           {
           expand_string_message =
             string_sprintf("internal expansion of \"%s\" failed: %s", sub,
               expand_string_message);
           goto EXPAND_FAILED;
           }
         yield = string_cat(yield, &size, &ptr, expanded, Ustrlen(expanded));
         continue;
         }
 
       case EOP_LC:
         {
         int count = 0;
         uschar *t = sub - 1;
         while (*(++t) != 0) { *t = tolower(*t); count++; }
         yield = string_cat(yield, &size, &ptr, sub, count);
         continue;
         }
 
       case EOP_UC:
         {
         int count = 0;
         uschar *t = sub - 1;
         while (*(++t) != 0) { *t = toupper(*t); count++; }
         yield = string_cat(yield, &size, &ptr, sub, count);
         continue;
         }
 
       case EOP_MD5:
 #ifdef SUPPORT_TLS
 	if (vp && *(void **)vp->value)
 	  {
 	  uschar * cp = tls_cert_fprt_md5(*(void **)vp->value);
 	  yield = string_cat(yield, &size, &ptr, cp, Ustrlen(cp));
 	  }
 	else
 #endif
 	  {
 	  md5 base;
 	  uschar digest[16];
 	  int j;
 	  char st[33];
 	  md5_start(&base);
 	  md5_end(&base, sub, Ustrlen(sub), digest);
 	  for(j = 0; j < 16; j++) sprintf(st+2*j, "%02x", digest[j]);
 	  yield = string_cat(yield, &size, &ptr, US st, (int)strlen(st));
 	  }
         continue;
 
       case EOP_SHA1:
 #ifdef SUPPORT_TLS
 	if (vp && *(void **)vp->value)
 	  {
 	  uschar * cp = tls_cert_fprt_sha1(*(void **)vp->value);
 	  yield = string_cat(yield, &size, &ptr, cp, Ustrlen(cp));
 	  }
 	else
 #endif
 	  {
 	  sha1 base;
 	  uschar digest[20];
 	  int j;
 	  char st[41];
 	  sha1_start(&base);
 	  sha1_end(&base, sub, Ustrlen(sub), digest);
 	  for(j = 0; j < 20; j++) sprintf(st+2*j, "%02X", digest[j]);
 	  yield = string_cat(yield, &size, &ptr, US st, (int)strlen(st));
 	  }
         continue;
 
       case EOP_SHA256:
 #ifdef SUPPORT_TLS
 	if (vp && *(void **)vp->value)
 	  {
 	  uschar * cp = tls_cert_fprt_sha256(*(void **)vp->value);
 	  yield = string_cat(yield, &size, &ptr, cp, (int)Ustrlen(cp));
 	  }
 	else
 #endif
 	  expand_string_message = US"sha256 only supported for certificates";
         continue;
 
        
 
       case EOP_HEX2B64:
         {
         int c = 0;
         int b = -1;
         uschar *in = sub;
         uschar *out = sub;
         uschar *enc;
 
         for (enc = sub; *enc != 0; enc++)
           {
           if (!isxdigit(*enc))
             {
             expand_string_message = string_sprintf("\"%s\" is not a hex "
               "string", sub);
             goto EXPAND_FAILED;
             }
           c++;
           }
 
         if ((c & 1) != 0)
           {
           expand_string_message = string_sprintf("\"%s\" contains an odd "
             "number of characters", sub);
           goto EXPAND_FAILED;
           }
 
         while ((c = *in++) != 0)
           {
           if (isdigit(c)) c -= '0';
           else c = toupper(c) - 'A' + 10;
           if (b == -1)
             {
             b = c << 4;
             }
           else
             {
             *out++ = b | c;
             b = -1;
             }
           }
 
         enc = auth_b64encode(sub, out - sub);
         yield = string_cat(yield, &size, &ptr, enc, Ustrlen(enc));
         continue;
         }
 
        
 
       case EOP_HEXQUOTE:
 	{
         uschar *t = sub - 1;
         while (*(++t) != 0)
           {
           if (*t < 0x21 || 0x7E < *t)
             yield = string_cat(yield, &size, &ptr,
 	      string_sprintf("\\x%02x", *t), 4);
 	  else
 	    yield = string_cat(yield, &size, &ptr, t, 1);
           }
 	continue;
 	}
 
        
 
       case EOP_LISTCOUNT:
         {
 	int cnt = 0;
 	int sep = 0;
 	uschar * cp;
 	uschar buffer[256];
 
 	while (string_nextinlist(&sub, &sep, buffer, sizeof(buffer)) != NULL) cnt++;
 	cp = string_sprintf("%d", cnt);
         yield = string_cat(yield, &size, &ptr, cp, Ustrlen(cp));
         continue;
         }
 
        
        
 
       case EOP_LISTNAMED:
 	{
 	tree_node *t = NULL;
 	uschar * list;
 	int sep = 0;
 	uschar * item;
 	uschar * suffix = US"";
 	BOOL needsep = FALSE;
 	uschar buffer[256];
 
 	if (*sub == '+') sub++;
 	if (arg == NULL)	 
 	  {
 	  if (!(t = tree_search(addresslist_anchor, sub)) &&
 	      !(t = tree_search(domainlist_anchor,  sub)) &&
 	      !(t = tree_search(hostlist_anchor,    sub)))
 	    t = tree_search(localpartlist_anchor, sub);
 	  }
 	else switch(*arg)	 
 	  {
 	  case 'a': t = tree_search(addresslist_anchor,   sub); suffix = US"_a"; break;
 	  case 'd': t = tree_search(domainlist_anchor,    sub); suffix = US"_d"; break;
 	  case 'h': t = tree_search(hostlist_anchor,      sub); suffix = US"_h"; break;
 	  case 'l': t = tree_search(localpartlist_anchor, sub); suffix = US"_l"; break;
 	  default:
             expand_string_message = string_sprintf("bad suffix on \"list\" operator");
 	    goto EXPAND_FAILED;
 	  }
 
 	if(!t)
 	  {
           expand_string_message = string_sprintf("\"%s\" is not a %snamed list",
             sub, !arg?""
 	      : *arg=='a'?"address "
 	      : *arg=='d'?"domain "
 	      : *arg=='h'?"host "
 	      : *arg=='l'?"localpart "
 	      : 0);
 	  goto EXPAND_FAILED;
 	  }
 
 	list = ((namedlist_block *)(t->data.ptr))->string;
 
 	while ((item = string_nextinlist(&list, &sep, buffer, sizeof(buffer))) != NULL)
 	  {
 	  uschar * buf = US" : ";
 	  if (needsep)
 	    yield = string_cat(yield, &size, &ptr, buf, 3);
 	  else
 	    needsep = TRUE;
 
 	  if (*item == '+')	 
 	    {
 	    uschar * sub = string_sprintf("${listnamed%s:%s}", suffix, item);
 	    item = expand_string_internal(sub, FALSE, NULL, FALSE, TRUE, &resetok);
 	    }
 	  else if (sep != ':')	 
 	    {
 	    char * cp;
 	    char tok[3];
 	    tok[0] = sep; tok[1] = ':'; tok[2] = 0;
 	    while ((cp= strpbrk((const char *)item, tok)))
 	      {
               yield = string_cat(yield, &size, &ptr, item, cp-(char *)item);
 	      if (*cp++ == ':')	 
 	        {
                 yield = string_cat(yield, &size, &ptr, US"::", 2);
 	        item = (uschar *)cp;
 		}
 	      else		 
 	        {
                 yield = string_cat(yield, &size, &ptr, (uschar *)tok, 1);
 		if (*cp == sep) cp++;
 	        item = (uschar *)cp;
 		}
 	      }
 	    }
           yield = string_cat(yield, &size, &ptr, item, Ustrlen(item));
 	  }
         continue;
 	}
 
        
 
       case EOP_MASK:
         {
         int count;
         uschar *endptr;
         int binary[4];
         int mask, maskoffset;
         int type = string_is_ip_address(sub, &maskoffset);
         uschar buffer[64];
 
         if (type == 0)
           {
           expand_string_message = string_sprintf("\"%s\" is not an IP address",
            sub);
           goto EXPAND_FAILED;
           }
 
         if (maskoffset == 0)
           {
           expand_string_message = string_sprintf("missing mask value in \"%s\"",
             sub);
           goto EXPAND_FAILED;
           }
 
         mask = Ustrtol(sub + maskoffset + 1, &endptr, 10);
 
         if (*endptr != 0 || mask < 0 || mask > ((type == 4)? 32 : 128))
           {
           expand_string_message = string_sprintf("mask value too big in \"%s\"",
             sub);
           goto EXPAND_FAILED;
           }
 
          
 
         sub[maskoffset] = 0;
         count = host_aton(sub, binary);
         host_mask(count, binary, mask);
 
          
 
         yield = string_cat(yield, &size, &ptr, buffer,
           host_nmtoa(count, binary, mask, buffer, '.'));
         continue;
         }
 
       case EOP_ADDRESS:
       case EOP_LOCAL_PART:
       case EOP_DOMAIN:
         {
         uschar *error;
         int start, end, domain;
         uschar *t = parse_extract_address(sub, &error, &start, &end, &domain,
           FALSE);
         if (t != NULL)
           {
           if (c != EOP_DOMAIN)
             {
             if (c == EOP_LOCAL_PART && domain != 0) end = start + domain - 1;
             yield = string_cat(yield, &size, &ptr, sub+start, end-start);
             }
           else if (domain != 0)
             {
             domain += start;
             yield = string_cat(yield, &size, &ptr, sub+domain, end-domain);
             }
           }
         continue;
         }
 
       case EOP_ADDRESSES:
         {
         uschar outsep[2] = { ':', '\0' };
         uschar *address, *error;
         int save_ptr = ptr;
         int start, end, domain;   
 
         while (isspace(*sub)) sub++;
         if (*sub == '>') { *outsep = *++sub; ++sub; }
         parse_allow_group = TRUE;
 
         for (;;)
           {
           uschar *p = parse_find_address_end(sub, FALSE);
           uschar saveend = *p;
           *p = '\0';
           address = parse_extract_address(sub, &error, &start, &end, &domain,
             FALSE);
           *p = saveend;
 
            
 
           if (address != NULL)
             {
             if (ptr != save_ptr && address[0] == *outsep)
               yield = string_cat(yield, &size, &ptr, US" ", 1);
 
             for (;;)
               {
               size_t seglen = Ustrcspn(address, outsep);
               yield = string_cat(yield, &size, &ptr, address, seglen + 1);
 
                
 
               if (address[seglen] == '\0') { ptr--; break; }
               yield = string_cat(yield, &size, &ptr, outsep, 1);
               address += seglen + 1;
               }
 
              
 
             yield = string_cat(yield, &size, &ptr, outsep, 1);
             }
 
           if (saveend == '\0') break;
           sub = p + 1;
           }
 
          
 
         if (ptr != save_ptr) ptr--;
         parse_allow_group = FALSE;
         continue;
         }
 
 
        
 
       case EOP_QUOTE:
       case EOP_QUOTE_LOCAL_PART:
       if (arg == NULL)
         {
         BOOL needs_quote = (*sub == 0);       
         uschar *t = sub - 1;
 
         if (c == EOP_QUOTE)
           {
           while (!needs_quote && *(++t) != 0)
             needs_quote = !isalnum(*t) && !strchr("_-.", *t);
           }
         else   
           {
           while (!needs_quote && *(++t) != 0)
             needs_quote = !isalnum(*t) &&
               strchr("!#$%&'*+-/=?^_`{|}~", *t) == NULL &&
               (*t != '.' || t == sub || t[1] == 0);
           }
 
         if (needs_quote)
           {
           yield = string_cat(yield, &size, &ptr, US"\"", 1);
           t = sub - 1;
           while (*(++t) != 0)
             {
             if (*t == '\n')
               yield = string_cat(yield, &size, &ptr, US"\\n", 2);
             else if (*t == '\r')
               yield = string_cat(yield, &size, &ptr, US"\\r", 2);
             else
               {
               if (*t == '\\' || *t == '"')
                 yield = string_cat(yield, &size, &ptr, US"\\", 1);
               yield = string_cat(yield, &size, &ptr, t, 1);
               }
             }
           yield = string_cat(yield, &size, &ptr, US"\"", 1);
           }
         else yield = string_cat(yield, &size, &ptr, sub, Ustrlen(sub));
         continue;
         }
 
        
 
       else
         {
         int n;
         uschar *opt = Ustrchr(arg, '_');
 
         if (opt != NULL) *opt++ = 0;
 
         n = search_findtype(arg, Ustrlen(arg));
         if (n < 0)
           {
           expand_string_message = search_error_message;
           goto EXPAND_FAILED;
           }
 
         if (lookup_list[n]->quote != NULL)
           sub = (lookup_list[n]->quote)(sub, opt);
         else if (opt != NULL) sub = NULL;
 
         if (sub == NULL)
           {
           expand_string_message = string_sprintf(
             "\"%s\" unrecognized after \"${quote_%s\"",
             opt, arg);
           goto EXPAND_FAILED;
           }
 
         yield = string_cat(yield, &size, &ptr, sub, Ustrlen(sub));
         continue;
         }
 
        
 
       case EOP_RXQUOTE:
         {
         uschar *t = sub - 1;
         while (*(++t) != 0)
           {
           if (!isalnum(*t))
             yield = string_cat(yield, &size, &ptr, US"\\", 1);
           yield = string_cat(yield, &size, &ptr, t, 1);
           }
         continue;
         }
 
        
 
       case EOP_RFC2047:
         {
         uschar buffer[2048];
         uschar *string = parse_quote_2047(sub, Ustrlen(sub), headers_charset,
           buffer, sizeof(buffer), FALSE);
         yield = string_cat(yield, &size, &ptr, string, Ustrlen(string));
         continue;
         }
 
        
 
       case EOP_RFC2047D:
         {
         int len;
         uschar *error;
         uschar *decoded = rfc2047_decode(sub, check_rfc2047_length,
           headers_charset, '?', &len, &error);
         if (error != NULL)
           {
           expand_string_message = error;
           goto EXPAND_FAILED;
           }
         yield = string_cat(yield, &size, &ptr, decoded, len);
         continue;
         }
 
        
 
       case EOP_FROM_UTF8:
         {
         while (*sub != 0)
           {
           int c;
           uschar buff[4];
           GETUTF8INC(c, sub);
           if (c > 255) c = '_';
           buff[0] = c;
           yield = string_cat(yield, &size, &ptr, buff, 1);
           }
         continue;
         }
 
 	   
 	  
       #define UTF8_REPLACEMENT_CHAR US"?"
 
       case EOP_UTF8CLEAN:
         {
         int seq_len, index = 0;
         int bytes_left = 0;
         uschar seq_buff[4];			 
         
         while (*sub != 0)
 	  {
 	  int complete;
 	  long codepoint;
 	  uschar c;
 
 	  complete = 0;
 	  c = *sub++;
 	  if (bytes_left)
 	    {
 	    if ((c & 0xc0) != 0x80)
 	      {
 		     
 	      complete = 1;  
 	      }
 	    else
 	      {
 	      codepoint = (codepoint << 6) | (c & 0x3f);
 	      seq_buff[index++] = c;
 	      if (--bytes_left == 0)		 
 		{
 		if(codepoint > 0x10FFFF)	 
 		  complete = -1;	 
 		else
 		  {		 
 		  yield = string_cat(yield, &size, &ptr, seq_buff, seq_len);
 		  index = 0;
 		  }
 		}
 	      }
 	    }
 	  else	 
 	    {
 	    if((c & 0x80) == 0)	 
 	      {
 	      yield = string_cat(yield, &size, &ptr, &c, 1);
 	      continue;
 	      }
 	    if((c & 0xe0) == 0xc0)		 
 	      {
 	      if(c == 0xc0 || c == 0xc1)	 
 		complete = -1;
 	      else
 		{
 		  bytes_left = 1;
 		  codepoint = c & 0x1f;
 		}
 	      }
 	    else if((c & 0xf0) == 0xe0)		 
 	      {
 	      bytes_left = 2;
 	      codepoint = c & 0x0f;
 	      }
 	    else if((c & 0xf8) == 0xf0)		 
 	      {
 	      bytes_left = 3;
 	      codepoint = c & 0x07;
 	      }
 	    else	 
 	      complete = -1;
 
 	    seq_buff[index++] = c;
 	    seq_len = bytes_left + 1;
 	    }		 
 
 	  if (complete != 0)
 	    {
 	    bytes_left = index = 0;
 	    yield = string_cat(yield, &size, &ptr, UTF8_REPLACEMENT_CHAR, 1);
 	    }
 	  if ((complete == 1) && ((c & 0x80) == 0))
 	    {  
 	      yield = string_cat(yield, &size, &ptr, &c, 1);
 	    }
 	  }
         continue;
         }
 
        
 
       case EOP_ESCAPE:
         {
         uschar *t = string_printing(sub);
         yield = string_cat(yield, &size, &ptr, t, Ustrlen(t));
         continue;
         }
 
        
 
       case EOP_EVAL:
       case EOP_EVAL10:
         {
         uschar *save_sub = sub;
         uschar *error = NULL;
         int_eximarith_t n = eval_expr(&sub, (c == EOP_EVAL10), &error, FALSE);
         if (error != NULL)
           {
           expand_string_message = string_sprintf("error in expression "
             "evaluation: %s (after processing \"%.*s\")", error, sub-save_sub,
               save_sub);
           goto EXPAND_FAILED;
           }
         sprintf(CS var_buffer, PR_EXIM_ARITH, n);
         yield = string_cat(yield, &size, &ptr, var_buffer, Ustrlen(var_buffer));
         continue;
         }
 
        
 
       case EOP_TIME_EVAL:
         {
         int n = readconf_readtime(sub, 0, FALSE);
         if (n < 0)
           {
           expand_string_message = string_sprintf("string \"%s\" is not an "
             "Exim time interval in \"%s\" operator", sub, name);
           goto EXPAND_FAILED;
           }
         sprintf(CS var_buffer, "%d", n);
         yield = string_cat(yield, &size, &ptr, var_buffer, Ustrlen(var_buffer));
         continue;
         }
 
       case EOP_TIME_INTERVAL:
         {
         int n;
         uschar *t = read_number(&n, sub);
         if (*t != 0)  
           {
           expand_string_message = string_sprintf("string \"%s\" is not a "
             "positive number in \"%s\" operator", sub, name);
           goto EXPAND_FAILED;
           }
         t = readconf_printtime(n);
         yield = string_cat(yield, &size, &ptr, t, Ustrlen(t));
         continue;
         }
 
        
 
       case EOP_STR2B64:
         {
         uschar *encstr = auth_b64encode(sub, Ustrlen(sub));
         yield = string_cat(yield, &size, &ptr, encstr, Ustrlen(encstr));
         continue;
         }
 
        
 
       case EOP_STRLEN:
         {
         uschar buff[24];
         (void)sprintf(CS buff, "%d", Ustrlen(sub));
         yield = string_cat(yield, &size, &ptr, buff, Ustrlen(buff));
         continue;
         }
 
        
 
       case EOP_LENGTH:
       case EOP_L:
       case EOP_SUBSTR:
       case EOP_S:
       case EOP_HASH:
       case EOP_H:
       case EOP_NHASH:
       case EOP_NH:
         {
         int sign = 1;
         int value1 = 0;
         int value2 = -1;
         int *pn;
         int len;
         uschar *ret;
 
         if (arg == NULL)
           {
           expand_string_message = string_sprintf("missing values after %s",
             name);
           goto EXPAND_FAILED;
           }
 
          
 
         if (c == EOP_LENGTH || c == EOP_L)
           {
           pn = &value2;
           value2 = 0;
           }
 
          
 
         else
           {
           pn = &value1;
           if (name[0] == 's' && *arg == '-') { sign = -1; arg++; }
           }
 
          
 
         ret = arg;
         while (*arg != 0)
           {
           if (arg != ret && *arg == '_' && pn == &value1)
             {
             pn = &value2;
             value2 = 0;
             if (arg[1] != 0) arg++;
             }
           else if (!isdigit(*arg))
             {
             expand_string_message =
               string_sprintf("non-digit after underscore in \"%s\"", name);
             goto EXPAND_FAILED;
             }
           else *pn = (*pn)*10 + *arg++ - '0';
           }
         value1 *= sign;
 
          
 
         ret =
           (c == EOP_HASH || c == EOP_H)?
              compute_hash(sub, value1, value2, &len) :
           (c == EOP_NHASH || c == EOP_NH)?
              compute_nhash(sub, value1, value2, &len) :
              extract_substr(sub, value1, value2, &len);
 
         if (ret == NULL) goto EXPAND_FAILED;
         yield = string_cat(yield, &size, &ptr, ret, len);
         continue;
         }
 
        
 
       case EOP_STAT:
         {
         uschar *s;
         uschar smode[12];
         uschar **modetable[3];
         int i;
         mode_t mode;
         struct stat st;
 
         if ((expand_forbid & RDO_EXISTS) != 0)
           {
           expand_string_message = US"Use of the stat() expansion is not permitted";
           goto EXPAND_FAILED;
           }
 
         if (stat(CS sub, &st) < 0)
           {
           expand_string_message = string_sprintf("stat(%s) failed: %s",
             sub, strerror(errno));
           goto EXPAND_FAILED;
           }
         mode = st.st_mode;
         switch (mode & S_IFMT)
           {
           case S_IFIFO: smode[0] = 'p'; break;
           case S_IFCHR: smode[0] = 'c'; break;
           case S_IFDIR: smode[0] = 'd'; break;
           case S_IFBLK: smode[0] = 'b'; break;
           case S_IFREG: smode[0] = '-'; break;
           default: smode[0] = '?'; break;
           }
 
         modetable[0] = ((mode & 01000) == 0)? mtable_normal : mtable_sticky;
         modetable[1] = ((mode & 02000) == 0)? mtable_normal : mtable_setid;
         modetable[2] = ((mode & 04000) == 0)? mtable_normal : mtable_setid;
 
         for (i = 0; i < 3; i++)
           {
           memcpy(CS(smode + 7 - i*3), CS(modetable[i][mode & 7]), 3);
           mode >>= 3;
           }
 
         smode[10] = 0;
         s = string_sprintf("mode=%04lo smode=%s inode=%ld device=%ld links=%ld "
           "uid=%ld gid=%ld size=" OFF_T_FMT " atime=%ld mtime=%ld ctime=%ld",
           (long)(st.st_mode & 077777), smode, (long)st.st_ino,
           (long)st.st_dev, (long)st.st_nlink, (long)st.st_uid,
           (long)st.st_gid, st.st_size, (long)st.st_atime,
           (long)st.st_mtime, (long)st.st_ctime);
         yield = string_cat(yield, &size, &ptr, s, Ustrlen(s));
         continue;
         }
 
        
 
       case EOP_RANDINT:
         {
          int_eximarith_t max;
          uschar *s;
  
        max = expand_string_integer(sub, TRUE);
//         max = expanded_string_integer(sub, TRUE);
          if (expand_string_message != NULL)
            goto EXPAND_FAILED;
          s = string_sprintf("%d", vaguely_random_number((int)max));
         yield = string_cat(yield, &size, &ptr, s, Ustrlen(s));
         continue;
         }
 
        
 
       case EOP_REVERSE_IP:
         {
         int family, maskptr;
         uschar reversed[128];
 
         family = string_is_ip_address(sub, &maskptr);
         if (family == 0)
           {
           expand_string_message = string_sprintf(
               "reverse_ip() not given an IP address [%s]", sub);
           goto EXPAND_FAILED;
           }
         invert_address(reversed, sub);
         yield = string_cat(yield, &size, &ptr, reversed, Ustrlen(reversed));
         continue;
         }
 
        
 
       default:
       expand_string_message =
         string_sprintf("unknown expansion operator \"%s\"", name);
       goto EXPAND_FAILED;
       }
     }
 
    
 						 
   if (*s++ == '}')
     {
     int len;
     int newsize = 0;
     if (ptr == 0)
       {
       if (resetok) store_reset(yield);
       yield = NULL;
       size = 0;
       }
     value = find_variable(name, FALSE, skipping, &newsize);
     if (value == NULL)
       {
       expand_string_message =
         string_sprintf("unknown variable in \"${%s}\"", name);
       check_variable_error_message(name);
       goto EXPAND_FAILED;
       }
     len = Ustrlen(value);
     if (yield == NULL && newsize != 0)
       {
       yield = value;
       size = newsize;
       ptr = len;
       }
     else yield = string_cat(yield, &size, &ptr, value, len);
     continue;
     }
 
    
 
   expand_string_message =
     string_sprintf("\"${%s\" is not a known operator (or a } is missing "
     "in a variable reference)", name);
   goto EXPAND_FAILED;
   }
 
  
 
 if (ket_ends && *s == 0)
   {
   expand_string_message = malformed_header?
     US"missing } at end of string - could be header name not terminated by colon"
     :
     US"missing } at end of string";
   goto EXPAND_FAILED;
   }
 
  
 
 if (yield == NULL) yield = store_get(1);
 yield[ptr] = 0;
 if (left != NULL) *left = s;
 
  
 
 if (resetok) store_reset(yield + ptr + 1);
 else if (resetok_p) *resetok_p = FALSE;
 
 DEBUG(D_expand)
   {
   debug_printf("expanding: %.*s\n   result: %s\n", (int)(s - string), string,
     yield);
   if (skipping) debug_printf("skipping: result is not used\n");
   }
 return yield;
 
  
 
 EXPAND_FAILED_CURLY:
 expand_string_message = malformed_header?
   US"missing or misplaced { or } - could be header name not terminated by colon"
   :
   US"missing or misplaced { or }";
 
  
 
 EXPAND_FAILED:
 if (left != NULL) *left = s;
 DEBUG(D_expand)
   {
   debug_printf("failed to expand: %s\n", string);
   debug_printf("   error message: %s\n", expand_string_message);
   if (expand_string_forcedfail) debug_printf("failure was forced\n");
   }
 if (resetok_p) *resetok_p = resetok;
 return NULL;
 }