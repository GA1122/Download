check_quotas(cupsd_client_t  *con,	 
             cupsd_printer_t *p)	 
{
  char		username[33],		 
		*name;			 
  cupsd_quota_t	*q;			 
#ifdef HAVE_MBR_UID_TO_UUID
  

  uuid_t	usr_uuid;		 
  uuid_t	usr2_uuid;		 
  uuid_t	grp_uuid;		 
  int		mbr_err;		 
  int		is_member;		 
#else
  

  struct passwd	*pw;			 
#endif  


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "check_quotas(%p[%d], %p[%s])",
                  con, con->number, p, p->name);

  

  strlcpy(username, get_username(con), sizeof(username));

  if ((name = strchr(username, '@')) != NULL)
    *name = '\0';			 

  

  if (MaxJobsPerPrinter)
  {
    

    if (cupsdGetPrinterJobCount(p->name) >= MaxJobsPerPrinter)
    {
      cupsdLogMessage(CUPSD_LOG_INFO, "Too many jobs for printer \"%s\"...",
                      p->name);
      return (-1);
    }
  }

  if (MaxJobsPerUser)
  {
    

    if (cupsdGetUserJobCount(username) >= MaxJobsPerUser)
    {
      cupsdLogMessage(CUPSD_LOG_INFO, "Too many jobs for user \"%s\"...",
                      username);
      return (-1);
    }
  }

  

  if (cupsArrayCount(p->users) == 0 && p->k_limit == 0 && p->page_limit == 0)
    return (1);

  if (cupsArrayCount(p->users))
  {
#ifdef HAVE_MBR_UID_TO_UUID
    

    if (mbr_user_name_to_uuid((char *)username, usr_uuid))
    {
      

      cupsdLogMessage(CUPSD_LOG_DEBUG,
		      "check_quotas: UUID lookup failed for user \"%s\"",
		      username);
      cupsdLogMessage(CUPSD_LOG_INFO,
		      "Denying user \"%s\" access to printer \"%s\" "
		      "(unknown user)...",
		      username, p->name);
      return (0);
    }
#else
    

    pw = getpwnam(username);
    endpwent();
#endif  

    for (name = (char *)cupsArrayFirst(p->users);
         name;
	 name = (char *)cupsArrayNext(p->users))
      if (name[0] == '@')
      {
        

#ifdef HAVE_MBR_UID_TO_UUID
        if (name[1] == '#')
	{
	  if (uuid_parse(name + 2, grp_uuid))
	    uuid_clear(grp_uuid);
	}
	else if ((mbr_err = mbr_group_name_to_uuid(name + 1, grp_uuid)) != 0)
	{
	  

	  cupsdLogMessage(CUPSD_LOG_DEBUG,
	                  "check_quotas: UUID lookup failed for ACL entry "
			  "\"%s\" (err=%d)", name, mbr_err);
	  cupsdLogMessage(CUPSD_LOG_WARN,
	                  "Access control entry \"%s\" not a valid group name; "
			  "entry ignored", name);
	}

	if ((mbr_err = mbr_check_membership(usr_uuid, grp_uuid,
					    &is_member)) != 0)
	{
	  

	  cupsdLogMessage(CUPSD_LOG_DEBUG,
			  "check_quotas: group \"%s\" membership check "
			  "failed (err=%d)", name + 1, mbr_err);
	  is_member = 0;
	}

        

	if (is_member)
	  break;

#else
        if (cupsdCheckGroup(username, pw, name + 1))
	  break;
#endif  
      }
#ifdef HAVE_MBR_UID_TO_UUID
      else
      {
        if (name[0] == '#')
	{
	  if (uuid_parse(name + 1, usr2_uuid))
	    uuid_clear(usr2_uuid);
        }
        else if ((mbr_err = mbr_user_name_to_uuid(name, usr2_uuid)) != 0)
    	{
	  

          cupsdLogMessage(CUPSD_LOG_DEBUG,
	                  "check_quotas: UUID lookup failed for ACL entry "
			  "\"%s\" (err=%d)", name, mbr_err);
          cupsdLogMessage(CUPSD_LOG_WARN,
	                  "Access control entry \"%s\" not a valid user name; "
			  "entry ignored", name);
	}

	if (!uuid_compare(usr_uuid, usr2_uuid))
	  break;
      }
#else
      else if (!_cups_strcasecmp(username, name))
	break;
#endif  

    if ((name != NULL) == p->deny_users)
    {
      cupsdLogMessage(CUPSD_LOG_INFO,
                      "Denying user \"%s\" access to printer \"%s\"...",
        	      username, p->name);
      return (0);
    }
  }

  

  if (p->k_limit || p->page_limit)
  {
    if ((q = cupsdUpdateQuota(p, username, 0, 0)) == NULL)
    {
      cupsdLogMessage(CUPSD_LOG_ERROR,
                      "Unable to allocate quota data for user \"%s\"",
                      username);
      return (-1);
    }

    if ((q->k_count >= p->k_limit && p->k_limit) ||
        (q->page_count >= p->page_limit && p->page_limit))
    {
      cupsdLogMessage(CUPSD_LOG_INFO, "User \"%s\" is over the quota limit...",
                      username);
      return (-1);
    }
  }

  

  return (1);
}
