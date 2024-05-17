  bgp_attr_unknown (struct bgp_attr_parser_args *args)
  {
  bgp_size_t total;
//   bgp_size_t total = args->total;
    struct transit *transit;
    struct attr_extra *attre;
    struct peer *const peer = args->peer; 
   struct attr *const attr = args->attr;
   u_char *const startp = args->startp;
   const u_char type = args->type;
   const u_char flag = args->flags;  
   const bgp_size_t length = args->length;
   
 
   if (BGP_DEBUG (normal, NORMAL))
   zlog_debug ("%s Unknown attribute is received (type %d, length %d)",
 	      peer->host, type, length);
   
   if (BGP_DEBUG (events, EVENTS))
     zlog (peer->log, LOG_DEBUG, 
 	  "Unknown attribute type %d length %d is received", type, length);
 
    
   stream_forward_getp (peer->ibuf, length);
 
    
   if (!CHECK_FLAG (flag, BGP_ATTR_FLAG_OPTIONAL))
     {
       return bgp_attr_malformed (args,
                                  BGP_NOTIFY_UPDATE_UNREC_ATTR,
                                  args->total);
     }
 
    
   if (! CHECK_FLAG (flag, BGP_ATTR_FLAG_TRANS))
     return BGP_ATTR_PARSE_PROCEED;
 
    
   SET_FLAG (*startp, BGP_ATTR_FLAG_PARTIAL);
 
    
   if (! ((attre = bgp_attr_extra_get(attr))->transit) )
       attre->transit = XCALLOC (MTYPE_TRANSIT, sizeof (struct transit));
 
   transit = attre->transit;
 
   if (transit->val)
     transit->val = XREALLOC (MTYPE_TRANSIT_VAL, transit->val, 
 			     transit->length + total);
   else
     transit->val = XMALLOC (MTYPE_TRANSIT_VAL, total);
 
   memcpy (transit->val + transit->length, startp, total);
   transit->length += total;
 
   return BGP_ATTR_PARSE_PROCEED;
 }