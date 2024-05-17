copy_attrs(ipp_t        *to,		 
           ipp_t        *from,		 
           cups_array_t *ra,		 
	   ipp_tag_t    group,		 
	   int          quickcopy,	 
	   cups_array_t *exclude)	 
{
  ipp_attribute_t	*fromattr;	 


  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "copy_attrs(to=%p, from=%p, ra=%p, group=%x, quickcopy=%d)",
		  to, from, ra, group, quickcopy);

  if (!to || !from)
    return;

  for (fromattr = from->attrs; fromattr; fromattr = fromattr->next)
  {
    

    if ((group != IPP_TAG_ZERO && fromattr->group_tag != group &&
         fromattr->group_tag != IPP_TAG_ZERO) || !fromattr->name)
      continue;

    if (!strcmp(fromattr->name, "document-password") ||
        !strcmp(fromattr->name, "job-authorization-uri") ||
        !strcmp(fromattr->name, "job-password") ||
        !strcmp(fromattr->name, "job-password-encryption") ||
        !strcmp(fromattr->name, "job-printer-uri"))
      continue;

    if (exclude &&
        (cupsArrayFind(exclude, fromattr->name) ||
	 cupsArrayFind(exclude, "all")))
    {
      

      if (strcmp(fromattr->name, "job-id"))
        continue;
    }

    if (!ra || cupsArrayFind(ra, fromattr->name))
    {
      

      if (fromattr->value_tag == IPP_TAG_BEGIN_COLLECTION &&
          !ra &&
	  (to->request.status.version[0] == 1 ||
	   !strcmp(fromattr->name, "media-col-database")))
	continue;

      ippCopyAttribute(to, fromattr, quickcopy);
    }
  }
}
