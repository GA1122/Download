 eval_condition(uschar *s, BOOL *resetok, BOOL *yield)
 {
 BOOL testfor = TRUE;
 BOOL tempcond, combined_cond;
 BOOL *subcondptr;
 BOOL sub2_honour_dollar = TRUE;
 int i, rc, cond_type, roffset;
 int_eximarith_t num[2];
 struct stat statbuf;
 uschar name[256];
 uschar *sub[10];
 
 const pcre *re;
 const uschar *rerror;
 
 for (;;)
   {
   while (isspace(*s)) s++;
   if (*s == '!') { testfor = !testfor; s++; } else break;
   }
 
  
 
 if (*s == '=' || *s == '>' || *s == '<')
   {
   int p = 0;
   name[p++] = *s++;
   if (*s == '=')
     {
     name[p++] = '=';
     s++;
     }
   name[p] = 0;
   }
 
  
 
 else s = read_name(name, 256, s, US"_");
 
  
 
 if (name[0] == 0)
   {
   expand_string_message = string_sprintf("condition name expected, "
     "but found \"%.16s\"", s);
   return NULL;
   }
 
  
 
 cond_type = chop_match(name, cond_table, sizeof(cond_table)/sizeof(uschar *));
 switch(cond_type)
   {
    
 
   case ECOND_DEF:
   if (*s != ':')
     {
     expand_string_message = US"\":\" expected after \"def\"";
     return NULL;
     }
 
   s = read_name(name, 256, s+1, US"_");
 
    
 
   if (Ustrncmp(name, "h_", 2) == 0 ||
       Ustrncmp(name, "rh_", 3) == 0 ||
       Ustrncmp(name, "bh_", 3) == 0 ||
       Ustrncmp(name, "header_", 7) == 0 ||
       Ustrncmp(name, "rheader_", 8) == 0 ||
       Ustrncmp(name, "bheader_", 8) == 0)
     {
     s = read_header_name(name, 256, s);
      
     if (Ustrchr(name, '}') != NULL) malformed_header = TRUE;
     if (yield != NULL) *yield =
       (find_header(name, TRUE, NULL, FALSE, NULL) != NULL) == testfor;
     }
 
    
 
   else
     {
     uschar *value = find_variable(name, TRUE, yield == NULL, NULL);
     if (value == NULL)
       {
       expand_string_message = (name[0] == 0)?
         string_sprintf("variable name omitted after \"def:\"") :
         string_sprintf("unknown variable \"%s\" after \"def:\"", name);
       check_variable_error_message(name);
       return NULL;
       }
     if (yield != NULL) *yield = (value[0] != 0) == testfor;
     }
 
   return s;
 
 
    
 
   case ECOND_FIRST_DELIVERY:
   if (yield != NULL) *yield = deliver_firsttime == testfor;
   return s;
 
 
    
 
   case ECOND_QUEUE_RUNNING:
   if (yield != NULL) *yield = (queue_run_pid != (pid_t)0) == testfor;
   return s;
 
 
    
 
   case ECOND_EXISTS:
   case ECOND_ISIP:
   case ECOND_ISIP4:
   case ECOND_ISIP6:
   case ECOND_PAM:
   case ECOND_RADIUS:
   case ECOND_LDAPAUTH:
   case ECOND_PWCHECK:
 
   while (isspace(*s)) s++;
   if (*s != '{') goto COND_FAILED_CURLY_START;		 
 
   sub[0] = expand_string_internal(s+1, TRUE, &s, yield == NULL, TRUE, resetok);
   if (sub[0] == NULL) return NULL;
    
   if (*s++ != '}') goto COND_FAILED_CURLY_END;
 
   if (yield == NULL) return s;    
 
   switch(cond_type)
     {
     case ECOND_EXISTS:
     if ((expand_forbid & RDO_EXISTS) != 0)
       {
       expand_string_message = US"File existence tests are not permitted";
       return NULL;
       }
     *yield = (Ustat(sub[0], &statbuf) == 0) == testfor;
     break;
 
     case ECOND_ISIP:
     case ECOND_ISIP4:
     case ECOND_ISIP6:
     rc = string_is_ip_address(sub[0], NULL);
     *yield = ((cond_type == ECOND_ISIP)? (rc != 0) :
              (cond_type == ECOND_ISIP4)? (rc == 4) : (rc == 6)) == testfor;
     break;
 
      
 
     case ECOND_PAM:
     #ifdef SUPPORT_PAM
     rc = auth_call_pam(sub[0], &expand_string_message);
     goto END_AUTH;
     #else
     goto COND_FAILED_NOT_COMPILED;
     #endif   
 
     case ECOND_RADIUS:
     #ifdef RADIUS_CONFIG_FILE
     rc = auth_call_radius(sub[0], &expand_string_message);
     goto END_AUTH;
     #else
     goto COND_FAILED_NOT_COMPILED;
     #endif   
 
     case ECOND_LDAPAUTH:
     #ifdef LOOKUP_LDAP
       {
        
       BOOL do_cache;
       int old_pool = store_pool;
       store_pool = POOL_SEARCH;
       rc = eldapauth_find((void *)(-1), NULL, sub[0], Ustrlen(sub[0]), NULL,
         &expand_string_message, &do_cache);
       store_pool = old_pool;
       }
     goto END_AUTH;
     #else
     goto COND_FAILED_NOT_COMPILED;
     #endif   
 
     case ECOND_PWCHECK:
     #ifdef CYRUS_PWCHECK_SOCKET
     rc = auth_call_pwcheck(sub[0], &expand_string_message);
     goto END_AUTH;
     #else
     goto COND_FAILED_NOT_COMPILED;
     #endif   
 
     #if defined(SUPPORT_PAM) || defined(RADIUS_CONFIG_FILE) || \
         defined(LOOKUP_LDAP) || defined(CYRUS_PWCHECK_SOCKET)
     END_AUTH:
     if (rc == ERROR || rc == DEFER) return NULL;
     *yield = (rc == OK) == testfor;
     #endif
     }
   return s;
 
 
    
 
   case ECOND_ACL:
      
     {
     uschar *user_msg;
     BOOL cond = FALSE;
     int size = 0;
     int ptr = 0;
 
     while (isspace(*s)) s++;
     if (*s++ != '{') goto COND_FAILED_CURLY_START;	 
 
     switch(read_subs(sub, sizeof(sub)/sizeof(*sub), 1,
       &s, yield == NULL, TRUE, US"acl", resetok))
       {
       case 1: expand_string_message = US"too few arguments or bracketing "
         "error for acl";
       case 2:
       case 3: return NULL;
       }
 
     *resetok = FALSE;
     if (yield != NULL) switch(eval_acl(sub, sizeof(sub)/sizeof(*sub), &user_msg))
 	{
 	case OK:
 	  cond = TRUE;
 	case FAIL:
           lookup_value = NULL;
 	  if (user_msg)
 	    {
             lookup_value = string_cat(NULL, &size, &ptr, user_msg, Ustrlen(user_msg));
             lookup_value[ptr] = '\0';
 	    }
 	  *yield = cond == testfor;
 	  break;
 
 	case DEFER:
           expand_string_forcedfail = TRUE;
 	default:
           expand_string_message = string_sprintf("error from acl \"%s\"", sub[0]);
 	  return NULL;
 	}
     return s;
     }
 
 
    
 
   case ECOND_SASLAUTHD:
   #ifndef CYRUS_SASLAUTHD_SOCKET
   goto COND_FAILED_NOT_COMPILED;
   #else
   while (isspace(*s)) s++;
   if (*s++ != '{') goto COND_FAILED_CURLY_START;	 
   switch(read_subs(sub, 4, 2, &s, yield == NULL, TRUE, US"saslauthd", resetok))
     {
     case 1: expand_string_message = US"too few arguments or bracketing "
       "error for saslauthd";
     case 2:
     case 3: return NULL;
     }
   if (sub[2] == NULL) sub[3] = NULL;   
   if (yield != NULL)
     {
     int rc;
     rc = auth_call_saslauthd(sub[0], sub[1], sub[2], sub[3],
       &expand_string_message);
     if (rc == ERROR || rc == DEFER) return NULL;
     *yield = (rc == OK) == testfor;
     }
   return s;
   #endif  
 
 
    
 
   case ECOND_MATCH_ADDRESS:
   case ECOND_MATCH_DOMAIN:
   case ECOND_MATCH_IP:
   case ECOND_MATCH_LOCAL_PART:
 #ifndef EXPAND_LISTMATCH_RHS
     sub2_honour_dollar = FALSE;
 #endif
      
 
   case ECOND_CRYPTEQ:
   case ECOND_INLIST:
   case ECOND_INLISTI:
   case ECOND_MATCH:
 
   case ECOND_NUM_L:      
   case ECOND_NUM_LE:
   case ECOND_NUM_E:
   case ECOND_NUM_EE:
   case ECOND_NUM_G:
   case ECOND_NUM_GE:
 
   case ECOND_STR_LT:     
   case ECOND_STR_LTI:
   case ECOND_STR_LE:
   case ECOND_STR_LEI:
   case ECOND_STR_EQ:
   case ECOND_STR_EQI:
   case ECOND_STR_GT:
   case ECOND_STR_GTI:
   case ECOND_STR_GE:
   case ECOND_STR_GEI:
 
   for (i = 0; i < 2; i++)
     {
      
     BOOL honour_dollar = TRUE;
     if ((i > 0) && !sub2_honour_dollar)
       honour_dollar = FALSE;
 
     while (isspace(*s)) s++;
     if (*s != '{')
       {
       if (i == 0) goto COND_FAILED_CURLY_START;
       expand_string_message = string_sprintf("missing 2nd string in {} "
         "after \"%s\"", name);
       return NULL;
       }
     sub[i] = expand_string_internal(s+1, TRUE, &s, yield == NULL,
         honour_dollar, resetok);
     if (sub[i] == NULL) return NULL;
     if (*s++ != '}') goto COND_FAILED_CURLY_END;
 
      
 
     if (!isalpha(name[0]) && yield != NULL)
       {
       if (sub[i][0] == 0)
         {
         num[i] = 0;
         DEBUG(D_expand)
           debug_printf("empty string cast to zero for numerical comparison\n");
          }
        else
          {
        num[i] = expand_string_integer(sub[i], FALSE);
//         num[i] = expanded_string_integer(sub[i], FALSE);
          if (expand_string_message != NULL) return NULL;
          }
        }
     }
 
    
 
   if (yield == NULL) return s;
 
    
 
   switch(cond_type)
     {
     case ECOND_NUM_E:
     case ECOND_NUM_EE:
     tempcond = (num[0] == num[1]);
     break;
 
     case ECOND_NUM_G:
     tempcond = (num[0] > num[1]);
     break;
 
     case ECOND_NUM_GE:
     tempcond = (num[0] >= num[1]);
     break;
 
     case ECOND_NUM_L:
     tempcond = (num[0] < num[1]);
     break;
 
     case ECOND_NUM_LE:
     tempcond = (num[0] <= num[1]);
     break;
 
     case ECOND_STR_LT:
     tempcond = (Ustrcmp(sub[0], sub[1]) < 0);
     break;
 
     case ECOND_STR_LTI:
     tempcond = (strcmpic(sub[0], sub[1]) < 0);
     break;
 
     case ECOND_STR_LE:
     tempcond = (Ustrcmp(sub[0], sub[1]) <= 0);
     break;
 
     case ECOND_STR_LEI:
     tempcond = (strcmpic(sub[0], sub[1]) <= 0);
     break;
 
     case ECOND_STR_EQ:
     tempcond = (Ustrcmp(sub[0], sub[1]) == 0);
     break;
 
     case ECOND_STR_EQI:
     tempcond = (strcmpic(sub[0], sub[1]) == 0);
     break;
 
     case ECOND_STR_GT:
     tempcond = (Ustrcmp(sub[0], sub[1]) > 0);
     break;
 
     case ECOND_STR_GTI:
     tempcond = (strcmpic(sub[0], sub[1]) > 0);
     break;
 
     case ECOND_STR_GE:
     tempcond = (Ustrcmp(sub[0], sub[1]) >= 0);
     break;
 
     case ECOND_STR_GEI:
     tempcond = (strcmpic(sub[0], sub[1]) >= 0);
     break;
 
     case ECOND_MATCH:    
     re = pcre_compile(CS sub[1], PCRE_COPT, (const char **)&rerror, &roffset,
       NULL);
     if (re == NULL)
       {
       expand_string_message = string_sprintf("regular expression error in "
         "\"%s\": %s at offset %d", sub[1], rerror, roffset);
       return NULL;
       }
     tempcond = regex_match_and_setup(re, sub[0], 0, -1);
     break;
 
     case ECOND_MATCH_ADDRESS:   
     rc = match_address_list(sub[0], TRUE, FALSE, &(sub[1]), NULL, -1, 0, NULL);
     goto MATCHED_SOMETHING;
 
     case ECOND_MATCH_DOMAIN:    
     rc = match_isinlist(sub[0], &(sub[1]), 0, &domainlist_anchor, NULL,
       MCL_DOMAIN + MCL_NOEXPAND, TRUE, NULL);
     goto MATCHED_SOMETHING;
 
     case ECOND_MATCH_IP:        
     if (sub[0][0] != 0 && string_is_ip_address(sub[0], NULL) == 0)
       {
       expand_string_message = string_sprintf("\"%s\" is not an IP address",
         sub[0]);
       return NULL;
       }
     else
       {
       unsigned int *nullcache = NULL;
       check_host_block cb;
 
       cb.host_name = US"";
       cb.host_address = sub[0];
 
        
 
       cb.host_ipv4 = (Ustrncmp(cb.host_address, "::ffff:", 7) == 0)?
         cb.host_address + 7 : cb.host_address;
 
       rc = match_check_list(
              &sub[1],                    
              0,                          
              &hostlist_anchor,           
              &nullcache,                 
              check_host,                 
              &cb,                        
              MCL_HOST,                   
              sub[0],                     
              NULL);                      
       }
     goto MATCHED_SOMETHING;
 
     case ECOND_MATCH_LOCAL_PART:
     rc = match_isinlist(sub[0], &(sub[1]), 0, &localpartlist_anchor, NULL,
       MCL_LOCALPART + MCL_NOEXPAND, TRUE, NULL);
      
      
     MATCHED_SOMETHING:
     switch(rc)
       {
       case OK:
       tempcond = TRUE;
       break;
 
       case FAIL:
       tempcond = FALSE;
       break;
 
       case DEFER:
       expand_string_message = string_sprintf("unable to complete match "
         "against \"%s\": %s", sub[1], search_error_message);
       return NULL;
       }
 
     break;
 
      
      
 
     case ECOND_CRYPTEQ:
     #ifndef SUPPORT_CRYPTEQ
     goto COND_FAILED_NOT_COMPILED;
     #else
     if (strncmpic(sub[1], US"{md5}", 5) == 0)
       {
       int sublen = Ustrlen(sub[1]+5);
       md5 base;
       uschar digest[16];
 
       md5_start(&base);
       md5_end(&base, (uschar *)sub[0], Ustrlen(sub[0]), digest);
 
        
 
       if (sublen == 24)
         {
         uschar *coded = auth_b64encode((uschar *)digest, 16);
         DEBUG(D_auth) debug_printf("crypteq: using MD5+B64 hashing\n"
           "  subject=%s\n  crypted=%s\n", coded, sub[1]+5);
         tempcond = (Ustrcmp(coded, sub[1]+5) == 0);
         }
       else if (sublen == 32)
         {
         int i;
         uschar coded[36];
         for (i = 0; i < 16; i++) sprintf(CS (coded+2*i), "%02X", digest[i]);
         coded[32] = 0;
         DEBUG(D_auth) debug_printf("crypteq: using MD5+hex hashing\n"
           "  subject=%s\n  crypted=%s\n", coded, sub[1]+5);
         tempcond = (strcmpic(coded, sub[1]+5) == 0);
         }
       else
         {
         DEBUG(D_auth) debug_printf("crypteq: length for MD5 not 24 or 32: "
           "fail\n  crypted=%s\n", sub[1]+5);
         tempcond = FALSE;
         }
       }
 
     else if (strncmpic(sub[1], US"{sha1}", 6) == 0)
       {
       int sublen = Ustrlen(sub[1]+6);
       sha1 base;
       uschar digest[20];
 
       sha1_start(&base);
       sha1_end(&base, (uschar *)sub[0], Ustrlen(sub[0]), digest);
 
        
 
       if (sublen == 28)
         {
         uschar *coded = auth_b64encode((uschar *)digest, 20);
         DEBUG(D_auth) debug_printf("crypteq: using SHA1+B64 hashing\n"
           "  subject=%s\n  crypted=%s\n", coded, sub[1]+6);
         tempcond = (Ustrcmp(coded, sub[1]+6) == 0);
         }
       else if (sublen == 40)
         {
         int i;
         uschar coded[44];
         for (i = 0; i < 20; i++) sprintf(CS (coded+2*i), "%02X", digest[i]);
         coded[40] = 0;
         DEBUG(D_auth) debug_printf("crypteq: using SHA1+hex hashing\n"
           "  subject=%s\n  crypted=%s\n", coded, sub[1]+6);
         tempcond = (strcmpic(coded, sub[1]+6) == 0);
         }
       else
         {
         DEBUG(D_auth) debug_printf("crypteq: length for SHA-1 not 28 or 40: "
           "fail\n  crypted=%s\n", sub[1]+6);
 	tempcond = FALSE;
         }
       }
 
     else    
             
       {
       int which = 0;
       uschar *coded;
 
       if (strncmpic(sub[1], US"{crypt}", 7) == 0)
         {
         sub[1] += 7;
         which = 1;
         }
       else if (strncmpic(sub[1], US"{crypt16}", 9) == 0)
         {
         sub[1] += 9;
         which = 2;
         }
       else if (sub[1][0] == '{')		 
         {
         expand_string_message = string_sprintf("unknown encryption mechanism "
           "in \"%s\"", sub[1]);
         return NULL;
         }
 
       switch(which)
         {
         case 0:  coded = US DEFAULT_CRYPT(CS sub[0], CS sub[1]); break;
         case 1:  coded = US crypt(CS sub[0], CS sub[1]); break;
         default: coded = US crypt16(CS sub[0], CS sub[1]); break;
         }
 
       #define STR(s) # s
       #define XSTR(s) STR(s)
       DEBUG(D_auth) debug_printf("crypteq: using %s()\n"
         "  subject=%s\n  crypted=%s\n",
         (which == 0)? XSTR(DEFAULT_CRYPT) : (which == 1)? "crypt" : "crypt16",
         coded, sub[1]);
       #undef STR
       #undef XSTR
 
        
 
       tempcond = (Ustrlen(sub[1]) < 2)? FALSE :
         (Ustrcmp(coded, sub[1]) == 0);
       }
     break;
     #endif   
 
     case ECOND_INLIST:
     case ECOND_INLISTI:
       {
       int sep = 0;
       uschar *save_iterate_item = iterate_item;
       int (*compare)(const uschar *, const uschar *);
 
       tempcond = FALSE;
       if (cond_type == ECOND_INLISTI)
         compare = strcmpic;
       else
         compare = (int (*)(const uschar *, const uschar *)) strcmp;
 
       while ((iterate_item = string_nextinlist(&sub[1], &sep, NULL, 0)) != NULL)
         if (compare(sub[0], iterate_item) == 0)
           {
           tempcond = TRUE;
           break;
           }
       iterate_item = save_iterate_item;
       }
 
     }    
 
   *yield = tempcond == testfor;
   return s;     
 
 
    
 
   case ECOND_AND:
   case ECOND_OR:
   subcondptr = (yield == NULL)? NULL : &tempcond;
   combined_cond = (cond_type == ECOND_AND);
 
   while (isspace(*s)) s++;
   if (*s++ != '{') goto COND_FAILED_CURLY_START;	 
 
   for (;;)
     {
     while (isspace(*s)) s++;
      
     if (*s == '}') break;
     if (*s != '{')					 
       {
       expand_string_message = string_sprintf("each subcondition "
         "inside an \"%s{...}\" condition must be in its own {}", name);
       return NULL;
       }
 
     if (!(s = eval_condition(s+1, resetok, subcondptr)))
       {
       expand_string_message = string_sprintf("%s inside \"%s{...}\" condition",
         expand_string_message, name);
       return NULL;
       }
     while (isspace(*s)) s++;
 
      
     if (*s++ != '}')
       {
        
       expand_string_message = string_sprintf("missing } at end of condition "
         "inside \"%s\" group", name);
       return NULL;
       }
 
     if (yield != NULL)
       {
       if (cond_type == ECOND_AND)
         {
         combined_cond &= tempcond;
         if (!combined_cond) subcondptr = NULL;   
         }                                        
       else
         {
         combined_cond |= tempcond;
         if (combined_cond) subcondptr = NULL;    
         }                                        
       }
     }
 
   if (yield != NULL) *yield = (combined_cond == testfor);
   return ++s;
 
 
    
 
   case ECOND_FORALL:
   case ECOND_FORANY:
     {
     int sep = 0;
     uschar *save_iterate_item = iterate_item;
 
     while (isspace(*s)) s++;
     if (*s++ != '{') goto COND_FAILED_CURLY_START;	 
     sub[0] = expand_string_internal(s, TRUE, &s, (yield == NULL), TRUE, resetok);
     if (sub[0] == NULL) return NULL;
      
     if (*s++ != '}') goto COND_FAILED_CURLY_END;
 
     while (isspace(*s)) s++;
     if (*s++ != '{') goto COND_FAILED_CURLY_START;	 
 
     sub[1] = s;
 
      
 
     if (!(s = eval_condition(sub[1], resetok, NULL)))
       {
       expand_string_message = string_sprintf("%s inside \"%s\" condition",
         expand_string_message, name);
       return NULL;
       }
     while (isspace(*s)) s++;
 
      
     if (*s++ != '}')
       {
        
       expand_string_message = string_sprintf("missing } at end of condition "
         "inside \"%s\"", name);
       return NULL;
       }
 
     if (yield != NULL) *yield = !testfor;
     while ((iterate_item = string_nextinlist(&sub[0], &sep, NULL, 0)) != NULL)
       {
       DEBUG(D_expand) debug_printf("%s: $item = \"%s\"\n", name, iterate_item);
       if (!eval_condition(sub[1], resetok, &tempcond))
         {
         expand_string_message = string_sprintf("%s inside \"%s\" condition",
           expand_string_message, name);
         iterate_item = save_iterate_item;
         return NULL;
         }
       DEBUG(D_expand) debug_printf("%s: condition evaluated to %s\n", name,
         tempcond? "true":"false");
 
       if (yield != NULL) *yield = (tempcond == testfor);
       if (tempcond == (cond_type == ECOND_FORANY)) break;
       }
 
     iterate_item = save_iterate_item;
     return s;
     }
 
 
    
   case ECOND_BOOL:
   case ECOND_BOOL_LAX:
     {
     uschar *sub_arg[1];
     uschar *t, *t2;
     uschar *ourname;
     size_t len;
     BOOL boolvalue = FALSE;
     while (isspace(*s)) s++;
     if (*s != '{') goto COND_FAILED_CURLY_START;	 
     ourname = cond_type == ECOND_BOOL_LAX ? US"bool_lax" : US"bool";
     switch(read_subs(sub_arg, 1, 1, &s, yield == NULL, FALSE, ourname, resetok))
       {
       case 1: expand_string_message = string_sprintf(
                   "too few arguments or bracketing error for %s",
                   ourname);
        
       case 2:
       case 3: return NULL;
       }
     t = sub_arg[0];
     while (isspace(*t)) t++;
     len = Ustrlen(t);
     if (len)
       {
        
       t2 = t + len - 1;
       while (isspace(*t2)) t2--;
       if (t2 != (t + len))
         {
         *++t2 = '\0';
         len = t2 - t;
         }
       }
     DEBUG(D_expand)
       debug_printf("considering %s: %s\n", ourname, len ? t : US"<empty>");
      
     if (len == 0)
       boolvalue = FALSE;
     else if (*t == '-'
 	     ? Ustrspn(t+1, "0123456789") == len-1
 	     : Ustrspn(t,   "0123456789") == len)
       {
       boolvalue = (Uatoi(t) == 0) ? FALSE : TRUE;
        
       if ((cond_type == ECOND_BOOL_LAX) && (len > 1))
         boolvalue = TRUE;
       }
     else if (strcmpic(t, US"true") == 0 || strcmpic(t, US"yes") == 0)
       boolvalue = TRUE;
     else if (strcmpic(t, US"false") == 0 || strcmpic(t, US"no") == 0)
       boolvalue = FALSE;
     else if (cond_type == ECOND_BOOL_LAX)
       boolvalue = TRUE;
     else
       {
       expand_string_message = string_sprintf("unrecognised boolean "
        "value \"%s\"", t);
       return NULL;
       }
     if (yield != NULL) *yield = (boolvalue == testfor);
     return s;
     }
 
    
 
   default:
   expand_string_message = string_sprintf("unknown condition \"%s\"", name);
   return NULL;
   }    
 
  
 
 COND_FAILED_CURLY_START:
 expand_string_message = string_sprintf("missing { after \"%s\"", name);
 return NULL;
 
 COND_FAILED_CURLY_END:
 expand_string_message = string_sprintf("missing } at end of \"%s\" condition",
   name);
 return NULL;
 
  
 
 #if !defined(SUPPORT_PAM) || !defined(RADIUS_CONFIG_FILE) || \
     !defined(LOOKUP_LDAP) || !defined(CYRUS_PWCHECK_SOCKET) || \
     !defined(SUPPORT_CRYPTEQ) || !defined(CYRUS_SASLAUTHD_SOCKET)
 COND_FAILED_NOT_COMPILED:
 expand_string_message = string_sprintf("support for \"%s\" not compiled",
   name);
 return NULL;
 #endif
 }