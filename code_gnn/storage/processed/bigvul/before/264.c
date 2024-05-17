bgp_attr_origin (struct bgp_attr_parser_args *args)
{
  struct peer *const peer = args->peer;
  struct attr *const attr = args->attr;
  const bgp_size_t length = args->length;
  
   
  if (length != 1)
    {
      zlog (peer->log, LOG_ERR, "Origin attribute length is not one %d",
	    length);
      return bgp_attr_malformed (args,
                                 BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
                                 args->total);
    }

   
  attr->origin = stream_getc (BGP_INPUT (peer));

   
  if ((attr->origin != BGP_ORIGIN_IGP)
      && (attr->origin != BGP_ORIGIN_EGP)
      && (attr->origin != BGP_ORIGIN_INCOMPLETE))
    {
      zlog (peer->log, LOG_ERR, "Origin attribute value is invalid %d",
	      attr->origin);
      return bgp_attr_malformed (args,
                                 BGP_NOTIFY_UPDATE_INVAL_ORIGIN,
                                 args->total);
    }

   
  attr->flag |= ATTR_FLAG_BIT (BGP_ATTR_ORIGIN);

  return 0;
}
