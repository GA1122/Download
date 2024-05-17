bgp_attr_munge_as4_attrs (struct peer *const peer,
                          struct attr *const attr,
                          struct aspath *as4_path, as_t as4_aggregator,
                          struct in_addr *as4_aggregator_addr)
{
  int ignore_as4_path = 0;
  struct aspath *newpath;
  struct attr_extra *attre = attr->extra;
    
  if (CHECK_FLAG (peer->cap, PEER_CAP_AS4_RCV))
    {
       
      if (BGP_DEBUG(as4, AS4))
        {
          if (attr->flag & (ATTR_FLAG_BIT(BGP_ATTR_AS4_PATH)))
            zlog_debug ("[AS4] %s %s AS4_PATH",
                        peer->host, "AS4 capable peer, yet it sent");
          
          if (attr->flag & (ATTR_FLAG_BIT(BGP_ATTR_AS4_AGGREGATOR)))
            zlog_debug ("[AS4] %s %s AS4_AGGREGATOR",
                        peer->host, "AS4 capable peer, yet it sent");
        }
      
      return BGP_ATTR_PARSE_PROCEED;
    }
  
   
  if (attr->flag & (ATTR_FLAG_BIT (BGP_ATTR_AS4_AGGREGATOR) ) )
    {
      if (attr->flag & (ATTR_FLAG_BIT (BGP_ATTR_AGGREGATOR) ) )
        {
          assert (attre);
          
           
          if (attre->aggregator_as != BGP_AS_TRANS)
            {
               
              if ( BGP_DEBUG(as4, AS4))
                zlog_debug ("[AS4] %s BGP not AS4 capable peer" 
                            " send AGGREGATOR != AS_TRANS and"
                            " AS4_AGGREGATOR, so ignore"
                            " AS4_AGGREGATOR and AS4_PATH", peer->host);
              ignore_as4_path = 1;
            }
          else
            {
               
              attre->aggregator_as = as4_aggregator;
              attre->aggregator_addr.s_addr = as4_aggregator_addr->s_addr;
            }
        }
      else
        {
           
          if ( BGP_DEBUG(as4, AS4))
            zlog_debug ("[AS4] %s BGP not AS4 capable peer send"
                        " AS4_AGGREGATOR but no AGGREGATOR, will take"
                        " it as if AGGREGATOR with AS_TRANS had been there", peer->host);
          (attre = bgp_attr_extra_get (attr))->aggregator_as = as4_aggregator;
           
          attr->flag |= (ATTR_FLAG_BIT (BGP_ATTR_AGGREGATOR));
        }
    }

   
  if (!ignore_as4_path && (attr->flag & (ATTR_FLAG_BIT( BGP_ATTR_AS4_PATH))))
    {
       newpath = aspath_reconcile_as4 (attr->aspath, as4_path);
       aspath_unintern (&attr->aspath);
       attr->aspath = aspath_intern (newpath);
    }
  return BGP_ATTR_PARSE_PROCEED;
}
