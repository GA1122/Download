bgp_attr_aspath_check (struct peer *const peer, struct attr *const attr)
{
   
  struct bgp *bgp = peer->bgp;
  struct aspath *aspath;

  bgp = peer->bgp;
    
   
  if ((peer_sort (peer) == BGP_PEER_CONFED && ! aspath_left_confed_check (attr->aspath)) ||
     (peer_sort (peer) == BGP_PEER_EBGP && aspath_confed_check (attr->aspath)))
    {
      zlog (peer->log, LOG_ERR, "Malformed AS path from %s", peer->host);
      bgp_notify_send (peer, BGP_NOTIFY_UPDATE_ERR,
                       BGP_NOTIFY_UPDATE_MAL_AS_PATH);
      return BGP_ATTR_PARSE_ERROR;
    }

   
  if (bgp != NULL && bgp_flag_check (bgp, BGP_FLAG_ENFORCE_FIRST_AS))
    {
      if (peer_sort (peer) == BGP_PEER_EBGP 
	  && ! aspath_firstas_check (attr->aspath, peer->as))
 	{
 	  zlog (peer->log, LOG_ERR,
 		"%s incorrect first AS (must be %u)", peer->host, peer->as);
          bgp_notify_send (peer, BGP_NOTIFY_UPDATE_ERR,
                           BGP_NOTIFY_UPDATE_MAL_AS_PATH);
          return BGP_ATTR_PARSE_ERROR;
 	}
    }

   
  if (peer->change_local_as &&
      ! CHECK_FLAG (peer->flags, PEER_FLAG_LOCAL_AS_NO_PREPEND))
    {
      aspath = aspath_dup (attr->aspath);
      aspath = aspath_add_seq (aspath, peer->change_local_as);
      aspath_unintern (&attr->aspath);
      attr->aspath = aspath_intern (aspath);
    }

  return BGP_ATTR_PARSE_PROCEED;
}
