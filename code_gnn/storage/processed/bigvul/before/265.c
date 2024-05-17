bgp_attr_parse (struct peer *peer, struct attr *attr, bgp_size_t size,
		struct bgp_nlri *mp_update, struct bgp_nlri *mp_withdraw)
{
  int ret;
  u_char flag = 0;
  u_char type = 0;
  bgp_size_t length;
  u_char *startp, *endp;
  u_char *attr_endp;
  u_char seen[BGP_ATTR_BITMAP_SIZE];
   
   
  struct aspath *as4_path = NULL;
  as_t as4_aggregator = 0;
  struct in_addr as4_aggregator_addr = { 0 };

   
  memset (seen, 0, BGP_ATTR_BITMAP_SIZE);

   
  endp = BGP_INPUT_PNT (peer) + size;
  
   
  while (BGP_INPUT_PNT (peer) < endp)
    {
       
      if (endp - BGP_INPUT_PNT (peer) < BGP_ATTR_MIN_LEN)
	{
	   
	  zlog (peer->log, LOG_WARNING, 
		"%s: error BGP attribute length %lu is smaller than min len",
		peer->host,
		(unsigned long) (endp - STREAM_PNT (BGP_INPUT (peer))));

	  bgp_notify_send (peer, 
			   BGP_NOTIFY_UPDATE_ERR, 
			   BGP_NOTIFY_UPDATE_ATTR_LENG_ERR);
	  return BGP_ATTR_PARSE_ERROR;
	}

       
      startp = BGP_INPUT_PNT (peer);
       
      flag = 0xF0 & stream_getc (BGP_INPUT (peer));
      type = stream_getc (BGP_INPUT (peer));

       
      if (CHECK_FLAG (flag, BGP_ATTR_FLAG_EXTLEN)
          && ((endp - startp) < (BGP_ATTR_MIN_LEN + 1)))
	{
	  zlog (peer->log, LOG_WARNING, 
		"%s: Extended length set, but just %lu bytes of attr header",
		peer->host,
		(unsigned long) (endp - STREAM_PNT (BGP_INPUT (peer))));

	  bgp_notify_send (peer, 
			   BGP_NOTIFY_UPDATE_ERR, 
			   BGP_NOTIFY_UPDATE_ATTR_LENG_ERR);
	  return BGP_ATTR_PARSE_ERROR;
	}
      
       
      if (CHECK_FLAG (flag, BGP_ATTR_FLAG_EXTLEN))
	length = stream_getw (BGP_INPUT (peer));
      else
	length = stream_getc (BGP_INPUT (peer));
      
       

      if (CHECK_BITMAP (seen, type))
	{
	  zlog (peer->log, LOG_WARNING,
		"%s: error BGP attribute type %d appears twice in a message",
		peer->host, type);

	  bgp_notify_send (peer, 
			   BGP_NOTIFY_UPDATE_ERR, 
			   BGP_NOTIFY_UPDATE_MAL_ATTR);
	  return BGP_ATTR_PARSE_ERROR;
	}

       

      SET_BITMAP (seen, type);

       
      attr_endp =  BGP_INPUT_PNT (peer) + length;

      if (attr_endp > endp)
	{
	  zlog (peer->log, LOG_WARNING, 
		"%s: BGP type %d length %d is too large, attribute total length is %d.  attr_endp is %p.  endp is %p", peer->host, type, length, size, attr_endp, endp);
	  bgp_notify_send (peer, 
			   BGP_NOTIFY_UPDATE_ERR, 
			   BGP_NOTIFY_UPDATE_ATTR_LENG_ERR);
	  return BGP_ATTR_PARSE_ERROR;
	}
	
        struct bgp_attr_parser_args attr_args = {
          .peer = peer,
          .length = length,
          .attr = attr,
          .type = type,
          .flags = flag,
          .startp = startp,
          .total = attr_endp - startp,
        };
      
	
       
      if (bgp_attr_flag_invalid (&attr_args))
        {
          bgp_attr_parse_ret_t ret;
          ret = bgp_attr_malformed (&attr_args,
                                    BGP_NOTIFY_UPDATE_ATTR_FLAG_ERR,
                                    attr_args.total);
          if (ret == BGP_ATTR_PARSE_PROCEED)
            continue;
          return ret;
        }

       
      switch (type)
	{
	case BGP_ATTR_ORIGIN:
	  ret = bgp_attr_origin (&attr_args);
	  break;
	case BGP_ATTR_AS_PATH:
	  ret = bgp_attr_aspath (&attr_args);
	  break;
	case BGP_ATTR_AS4_PATH:
	  ret = bgp_attr_as4_path (&attr_args, &as4_path);
	  break;
	case BGP_ATTR_NEXT_HOP:	
	  ret = bgp_attr_nexthop (&attr_args);
	  break;
	case BGP_ATTR_MULTI_EXIT_DISC:
	  ret = bgp_attr_med (&attr_args);
	  break;
	case BGP_ATTR_LOCAL_PREF:
	  ret = bgp_attr_local_pref (&attr_args);
	  break;
	case BGP_ATTR_ATOMIC_AGGREGATE:
	  ret = bgp_attr_atomic (&attr_args);
	  break;
	case BGP_ATTR_AGGREGATOR:
	  ret = bgp_attr_aggregator (&attr_args);
	  break;
	case BGP_ATTR_AS4_AGGREGATOR:
	  ret = bgp_attr_as4_aggregator (&attr_args,
	                                 &as4_aggregator,
	                                 &as4_aggregator_addr);
	  break;
	case BGP_ATTR_COMMUNITIES:
	  ret = bgp_attr_community (&attr_args);
	  break;
	case BGP_ATTR_ORIGINATOR_ID:
	  ret = bgp_attr_originator_id (&attr_args);
	  break;
	case BGP_ATTR_CLUSTER_LIST:
	  ret = bgp_attr_cluster_list (&attr_args);
	  break;
	case BGP_ATTR_MP_REACH_NLRI:
	  ret = bgp_mp_reach_parse (&attr_args, mp_update);
	  break;
	case BGP_ATTR_MP_UNREACH_NLRI:
	  ret = bgp_mp_unreach_parse (&attr_args, mp_withdraw);
	  break;
	case BGP_ATTR_EXT_COMMUNITIES:
	  ret = bgp_attr_ext_communities (&attr_args);
	  break;
	default:
	  ret = bgp_attr_unknown (&attr_args);
	  break;
	}
      
       
      if (ret == BGP_ATTR_PARSE_ERROR)
        {
          zlog (peer->log, LOG_WARNING,
                "%s: Attribute %s, parse error", 
                peer->host, 
                LOOKUP (attr_str, type));
          bgp_notify_send (peer, 
                           BGP_NOTIFY_UPDATE_ERR,
                           BGP_NOTIFY_UPDATE_MAL_ATTR);
          if (as4_path)
            aspath_unintern (&as4_path);
          return ret;
        }
      if (ret == BGP_ATTR_PARSE_WITHDRAW)
        {
          
          zlog (peer->log, LOG_WARNING,
                "%s: Attribute %s, parse error - treating as withdrawal",
                peer->host,
                LOOKUP (attr_str, type));
          if (as4_path)
            aspath_unintern (&as4_path);
          return ret;
        }
      
       
      if (BGP_INPUT_PNT (peer) != attr_endp)
	{
	  zlog (peer->log, LOG_WARNING, 
		"%s: BGP attribute %s, fetch error", 
                peer->host, LOOKUP (attr_str, type));
	  bgp_notify_send (peer, 
			   BGP_NOTIFY_UPDATE_ERR, 
			   BGP_NOTIFY_UPDATE_ATTR_LENG_ERR);
          if (as4_path)
            aspath_unintern (&as4_path);
	  return BGP_ATTR_PARSE_ERROR;
	}
    }

   
  if (BGP_INPUT_PNT (peer) != endp)
    {
      zlog (peer->log, LOG_WARNING, 
	    "%s: BGP attribute %s, length mismatch",
	    peer->host, LOOKUP (attr_str, type));
      bgp_notify_send (peer, 
		       BGP_NOTIFY_UPDATE_ERR, 
		       BGP_NOTIFY_UPDATE_ATTR_LENG_ERR);
      if (as4_path)
        aspath_unintern (&as4_path);
      return BGP_ATTR_PARSE_ERROR;
    }

   
  if (bgp_attr_munge_as4_attrs (peer, attr, as4_path,
                                as4_aggregator, &as4_aggregator_addr))
    {
      if (as4_path)
        aspath_unintern (&as4_path);
      return BGP_ATTR_PARSE_ERROR;
    }

   
  if (as4_path)
    {
      aspath_unintern (&as4_path);  
       
    }
   
   
  if (attr->flag & (ATTR_FLAG_BIT(BGP_ATTR_AS_PATH)))
    {
      ret = bgp_attr_aspath_check (peer, attr);
      if (ret != BGP_ATTR_PARSE_PROCEED)
	return ret;
    }

   
  if (attr->extra && attr->extra->transit)
    attr->extra->transit = transit_intern (attr->extra->transit);

  return BGP_ATTR_PARSE_PROCEED;
}
