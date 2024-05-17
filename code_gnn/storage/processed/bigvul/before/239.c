bgp_attr_aspath (struct bgp_attr_parser_args *args)
{
  struct attr *const attr = args->attr;
  struct peer *const peer = args->peer; 
  const bgp_size_t length = args->length;
  
   
  attr->aspath = aspath_parse (peer->ibuf, length, 
                               CHECK_FLAG (peer->cap, PEER_CAP_AS4_RCV));

   
  if (! attr->aspath)
    {
      zlog (peer->log, LOG_ERR,
            "Malformed AS path from %s, length is %d",
            peer->host, length);
      return bgp_attr_malformed (args, BGP_NOTIFY_UPDATE_MAL_AS_PATH, 0);
    }

   
  attr->flag |= ATTR_FLAG_BIT (BGP_ATTR_AS_PATH);

  return BGP_ATTR_PARSE_PROCEED;
}
