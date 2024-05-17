find_variable(uschar *name, BOOL exists_only, BOOL skipping, int *newsize)
{
var_entry * vp;
uschar *s, *domain;
uschar **ss;
void * val;

 

if ((Ustrncmp(name, "acl_c", 5) == 0 || Ustrncmp(name, "acl_m", 5) == 0) &&
     !isalpha(name[5]))
  {
  tree_node *node =
    tree_search((name[4] == 'c')? acl_var_c : acl_var_m, name + 4);
  return (node == NULL)? (strict_acl_vars? NULL : US"") : node->data.ptr;
  }

 

if (Ustrncmp(name, "auth", 4) == 0)
  {
  uschar *endptr;
  int n = Ustrtoul(name + 4, &endptr, 10);
  if (*endptr == 0 && n != 0 && n <= AUTH_VARS)
    return (auth_vars[n-1] == NULL)? US"" : auth_vars[n-1];
  }

 

if (!(vp = find_var_ent(name)))
  return NULL;           

 

if (skipping)
  return US"";

val = vp->value;
switch (vp->type)
  {
  case vtype_filter_int:
  if (!filter_running) return NULL;
   
   
  case vtype_int:
  sprintf(CS var_buffer, "%d", *(int *)(val));  
  return var_buffer;

  case vtype_ino:
  sprintf(CS var_buffer, "%ld", (long int)(*(ino_t *)(val)));  
  return var_buffer;

  case vtype_gid:
  sprintf(CS var_buffer, "%ld", (long int)(*(gid_t *)(val)));  
  return var_buffer;

  case vtype_uid:
  sprintf(CS var_buffer, "%ld", (long int)(*(uid_t *)(val)));  
  return var_buffer;

  case vtype_bool:
  sprintf(CS var_buffer, "%s", *(BOOL *)(val) ? "yes" : "no");  
  return var_buffer;

  case vtype_stringptr:                       
  s = *((uschar **)(val));
  return (s == NULL)? US"" : s;

  case vtype_pid:
  sprintf(CS var_buffer, "%d", (int)getpid());  
  return var_buffer;

  case vtype_load_avg:
  sprintf(CS var_buffer, "%d", OS_GETLOADAVG());  
  return var_buffer;

  case vtype_host_lookup:                     
  if (sender_host_name == NULL && sender_host_address != NULL &&
      !host_lookup_failed && host_name_lookup() == OK)
    host_build_sender_fullhost();
  return (sender_host_name == NULL)? US"" : sender_host_name;

  case vtype_localpart:                       
  s = *((uschar **)(val));
  if (s == NULL) return US"";
  domain = Ustrrchr(s, '@');
  if (domain == NULL) return s;
  if (domain - s > sizeof(var_buffer) - 1)
    log_write(0, LOG_MAIN|LOG_PANIC_DIE, "local part longer than " SIZE_T_FMT
	" in string expansion", sizeof(var_buffer));
  Ustrncpy(var_buffer, s, domain - s);
  var_buffer[domain - s] = 0;
  return var_buffer;

  case vtype_domain:                          
  s = *((uschar **)(val));
  if (s == NULL) return US"";
  domain = Ustrrchr(s, '@');
  return (domain == NULL)? US"" : domain + 1;

  case vtype_msgheaders:
  return find_header(NULL, exists_only, newsize, FALSE, NULL);

  case vtype_msgheaders_raw:
  return find_header(NULL, exists_only, newsize, TRUE, NULL);

  case vtype_msgbody:                         
  case vtype_msgbody_end:                     
  ss = (uschar **)(val);
  if (*ss == NULL && deliver_datafile >= 0)   
    {
    uschar *body;
    off_t start_offset = SPOOL_DATA_START_OFFSET;
    int len = message_body_visible;
    if (len > message_size) len = message_size;
    *ss = body = store_malloc(len+1);
    body[0] = 0;
    if (vp->type == vtype_msgbody_end)
      {
      struct stat statbuf;
      if (fstat(deliver_datafile, &statbuf) == 0)
	{
	start_offset = statbuf.st_size - len;
	if (start_offset < SPOOL_DATA_START_OFFSET)
	  start_offset = SPOOL_DATA_START_OFFSET;
	}
      }
    lseek(deliver_datafile, start_offset, SEEK_SET);
    len = read(deliver_datafile, body, len);
    if (len > 0)
      {
      body[len] = 0;
      if (message_body_newlines)    
	{
	while (len > 0)
	  { if (body[--len] == 0) body[len] = ' '; }
	}
      else
	{
	while (len > 0)
	  { if (body[--len] == '\n' || body[len] == 0) body[len] = ' '; }
	}
      }
    }
  return (*ss == NULL)? US"" : *ss;

  case vtype_todbsdin:                        
  return tod_stamp(tod_bsdin);

  case vtype_tode:                            
  return tod_stamp(tod_epoch);

  case vtype_todel:                           
  return tod_stamp(tod_epoch_l);

  case vtype_todf:                            
  return tod_stamp(tod_full);

  case vtype_todl:                            
  return tod_stamp(tod_log_bare);             

  case vtype_todzone:                         
  return tod_stamp(tod_zone);

  case vtype_todzulu:                         
  return tod_stamp(tod_zulu);

  case vtype_todlf:                           
  return tod_stamp(tod_log_datestamp_daily);

  case vtype_reply:                           
  s = find_header(US"reply-to:", exists_only, newsize, TRUE,
    headers_charset);
  if (s != NULL) while (isspace(*s)) s++;
  if (s == NULL || *s == 0)
    {
    *newsize = 0;                             
    s = find_header(US"from:", exists_only, newsize, TRUE, headers_charset);
    }
  if (s != NULL)
    {
    uschar *t;
    while (isspace(*s)) s++;
    for (t = s; *t != 0; t++) if (*t == '\n') *t = ' ';
    while (t > s && isspace(t[-1])) t--;
    *t = 0;
    }
  return (s == NULL)? US"" : s;

  case vtype_string_func:
    {
    uschar * (*fn)() = val;
    return fn();
    }

  case vtype_pspace:
    {
    int inodes;
    sprintf(CS var_buffer, "%d",
      receive_statvfs(val == (void *)TRUE, &inodes));
    }
  return var_buffer;

  case vtype_pinodes:
    {
    int inodes;
    (void) receive_statvfs(val == (void *)TRUE, &inodes);
    sprintf(CS var_buffer, "%d", inodes);
    }
  return var_buffer;

  case vtype_cert:
  return *(void **)val ? US"<cert>" : US"";

  #ifndef DISABLE_DKIM
  case vtype_dkim:
  return dkim_exim_expand_query((int)(long)val);
  #endif

  }
}
