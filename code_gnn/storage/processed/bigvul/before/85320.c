check_rss_recipient(
    const char *recipient)		 
{
  cupsd_subscription_t	*sub;		 


  for (sub = (cupsd_subscription_t *)cupsArrayFirst(Subscriptions);
       sub;
       sub = (cupsd_subscription_t *)cupsArrayNext(Subscriptions))
    if (sub->recipient)
    {
      

      const char *r1, *r2;

      for (r1 = recipient, r2 = sub->recipient;
           *r1 == *r2 && *r1 && *r1 != '?' && *r2 && *r2 != '?';
	   r1 ++, r2 ++);

      if (*r1 == *r2)
        return (0);
    }

  return (1);
}
