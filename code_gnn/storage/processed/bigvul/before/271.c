bgp_packet_attribute (struct bgp *bgp, struct peer *peer,
		      struct stream *s, struct attr *attr, struct prefix *p,
		      afi_t afi, safi_t safi, struct peer *from,
		      struct prefix_rd *prd, u_char *tag)
{
  size_t cp;
  size_t aspath_sizep;
  struct aspath *aspath;
  int send_as4_path = 0;
  int send_as4_aggregator = 0;
  int use32bit = (CHECK_FLAG (peer->cap, PEER_CAP_AS4_RCV)) ? 1 : 0;

  if (! bgp)
    bgp = bgp_get_default ();

   
  cp = stream_get_endp (s);

   
  stream_putc (s, BGP_ATTR_FLAG_TRANS);
  stream_putc (s, BGP_ATTR_ORIGIN);
  stream_putc (s, 1);
  stream_putc (s, attr->origin);

   

   
  if (peer_sort (peer) == BGP_PEER_EBGP
      && (! CHECK_FLAG (peer->af_flags[afi][safi], PEER_FLAG_AS_PATH_UNCHANGED)
	  || attr->aspath->segments == NULL)
      && (! CHECK_FLAG (peer->af_flags[afi][safi], PEER_FLAG_RSERVER_CLIENT)))
    {    
      aspath = aspath_dup (attr->aspath);

      if (CHECK_FLAG(bgp->config, BGP_CONFIG_CONFEDERATION))
	{
	   
	  aspath = aspath_delete_confed_seq (aspath);
	  aspath = aspath_add_seq (aspath, bgp->confed_id);
	}
      else
	{
	  aspath = aspath_add_seq (aspath, peer->local_as);
	  if (peer->change_local_as)
	    aspath = aspath_add_seq (aspath, peer->change_local_as);
	}
    }
  else if (peer_sort (peer) == BGP_PEER_CONFED)
    {
       
      aspath = aspath_dup (attr->aspath);
      aspath = aspath_add_confed_seq (aspath, peer->local_as);
    }
  else
    aspath = attr->aspath;

   

  stream_putc (s, BGP_ATTR_FLAG_TRANS|BGP_ATTR_FLAG_EXTLEN);
  stream_putc (s, BGP_ATTR_AS_PATH);
  aspath_sizep = stream_get_endp (s);
  stream_putw (s, 0);
  stream_putw_at (s, aspath_sizep, aspath_put (s, aspath, use32bit));
  
   
  if (!use32bit && aspath_has_as4 (aspath))
      send_as4_path = 1;  
  
   
  if (attr->flag & ATTR_FLAG_BIT (BGP_ATTR_NEXT_HOP) && afi == AFI_IP)
    {
      stream_putc (s, BGP_ATTR_FLAG_TRANS);
      stream_putc (s, BGP_ATTR_NEXT_HOP);
      stream_putc (s, 4);
      if (safi == SAFI_MPLS_VPN)
	{
	  if (attr->nexthop.s_addr == 0)
	    stream_put_ipv4 (s, peer->nexthop.v4.s_addr);
	  else
	    stream_put_ipv4 (s, attr->nexthop.s_addr);
	}
      else
	stream_put_ipv4 (s, attr->nexthop.s_addr);
    }

   
  if (attr->flag & ATTR_FLAG_BIT (BGP_ATTR_MULTI_EXIT_DISC))
    {
      stream_putc (s, BGP_ATTR_FLAG_OPTIONAL);
      stream_putc (s, BGP_ATTR_MULTI_EXIT_DISC);
      stream_putc (s, 4);
      stream_putl (s, attr->med);
    }

   
  if (peer_sort (peer) == BGP_PEER_IBGP ||
      peer_sort (peer) == BGP_PEER_CONFED)
    {
      stream_putc (s, BGP_ATTR_FLAG_TRANS);
      stream_putc (s, BGP_ATTR_LOCAL_PREF);
      stream_putc (s, 4);
      stream_putl (s, attr->local_pref);
    }

   
  if (attr->flag & ATTR_FLAG_BIT (BGP_ATTR_ATOMIC_AGGREGATE))
    {
      stream_putc (s, BGP_ATTR_FLAG_TRANS);
      stream_putc (s, BGP_ATTR_ATOMIC_AGGREGATE);
      stream_putc (s, 0);
    }

   
  if (attr->flag & ATTR_FLAG_BIT (BGP_ATTR_AGGREGATOR))
    {
      assert (attr->extra);
      
       
      stream_putc (s, BGP_ATTR_FLAG_OPTIONAL|BGP_ATTR_FLAG_TRANS);
      stream_putc (s, BGP_ATTR_AGGREGATOR);
      
      if (use32bit)
        {
           
          stream_putc (s, 8);
          stream_putl (s, attr->extra->aggregator_as);
        }
      else
        {
           
          stream_putc (s, 6);
          
           
          if ( attr->extra->aggregator_as > 65535 )
            {
              stream_putw (s, BGP_AS_TRANS);
              
               
              send_as4_aggregator = 1;
            }
          else
            stream_putw (s, (u_int16_t) attr->extra->aggregator_as);
        }
      stream_put_ipv4 (s, attr->extra->aggregator_addr.s_addr);
    }

   
  if (CHECK_FLAG (peer->af_flags[afi][safi], PEER_FLAG_SEND_COMMUNITY) 
      && (attr->flag & ATTR_FLAG_BIT (BGP_ATTR_COMMUNITIES)))
    {
      if (attr->community->size * 4 > 255)
	{
	  stream_putc (s, BGP_ATTR_FLAG_OPTIONAL|BGP_ATTR_FLAG_TRANS|BGP_ATTR_FLAG_EXTLEN);
	  stream_putc (s, BGP_ATTR_COMMUNITIES);
	  stream_putw (s, attr->community->size * 4);
	}
      else
	{
	  stream_putc (s, BGP_ATTR_FLAG_OPTIONAL|BGP_ATTR_FLAG_TRANS);
	  stream_putc (s, BGP_ATTR_COMMUNITIES);
	  stream_putc (s, attr->community->size * 4);
	}
      stream_put (s, attr->community->val, attr->community->size * 4);
    }

   
  if (peer_sort (peer) == BGP_PEER_IBGP
      && from
      && peer_sort (from) == BGP_PEER_IBGP)
    {
       
      stream_putc (s, BGP_ATTR_FLAG_OPTIONAL);
      stream_putc (s, BGP_ATTR_ORIGINATOR_ID);
      stream_putc (s, 4);

      if (attr->flag & ATTR_FLAG_BIT(BGP_ATTR_ORIGINATOR_ID))
	stream_put_in_addr (s, &attr->extra->originator_id);
      else 
        stream_put_in_addr (s, &from->remote_id);

       
      stream_putc (s, BGP_ATTR_FLAG_OPTIONAL);
      stream_putc (s, BGP_ATTR_CLUSTER_LIST);
      
      if (attr->extra && attr->extra->cluster)
	{
	  stream_putc (s, attr->extra->cluster->length + 4);
	   
	  if (bgp->config & BGP_CONFIG_CLUSTER_ID)
	    stream_put_in_addr (s, &bgp->cluster_id);
	  else
	    stream_put_in_addr (s, &bgp->router_id);
	  stream_put (s, attr->extra->cluster->list, 
	              attr->extra->cluster->length);
	}
      else
	{
	  stream_putc (s, 4);
	   
	  if (bgp->config & BGP_CONFIG_CLUSTER_ID)
	    stream_put_in_addr (s, &bgp->cluster_id);
	  else
	    stream_put_in_addr (s, &bgp->router_id);
	}
    }

#ifdef HAVE_IPV6
   
  if (p->family == AF_INET6)
    {
      unsigned long sizep;
      struct attr_extra *attre = attr->extra;
      
      assert (attr->extra);
      
      stream_putc (s, BGP_ATTR_FLAG_OPTIONAL);
      stream_putc (s, BGP_ATTR_MP_REACH_NLRI);
      sizep = stream_get_endp (s);
      stream_putc (s, 0);	 
      stream_putw (s, AFI_IP6);	 
      stream_putc (s, safi);	 

      stream_putc (s, attre->mp_nexthop_len);

      if (attre->mp_nexthop_len == 16)
	stream_put (s, &attre->mp_nexthop_global, 16);
      else if (attre->mp_nexthop_len == 32)
	{
	  stream_put (s, &attre->mp_nexthop_global, 16);
	  stream_put (s, &attre->mp_nexthop_local, 16);
	}
      
       
      stream_putc (s, 0);

       
      stream_put_prefix (s, p);

       
      stream_putc_at (s, sizep, (stream_get_endp (s) - sizep) - 1);
    }
#endif  

  if (p->family == AF_INET && safi == SAFI_MULTICAST)
    {
      unsigned long sizep;

      stream_putc (s, BGP_ATTR_FLAG_OPTIONAL);
      stream_putc (s, BGP_ATTR_MP_REACH_NLRI);
      sizep = stream_get_endp (s);
      stream_putc (s, 0);	 
      stream_putw (s, AFI_IP);	 
      stream_putc (s, SAFI_MULTICAST);	 

      stream_putc (s, 4);
      stream_put_ipv4 (s, attr->nexthop.s_addr);

       
      stream_putc (s, 0);

       
      stream_put_prefix (s, p);

       
      stream_putc_at (s, sizep, (stream_get_endp (s) - sizep) - 1);
    }

  if (p->family == AF_INET && safi == SAFI_MPLS_VPN)
    {
      unsigned long sizep;

      stream_putc (s, BGP_ATTR_FLAG_OPTIONAL);
      stream_putc (s, BGP_ATTR_MP_REACH_NLRI);
      sizep = stream_get_endp (s);
      stream_putc (s, 0);	 
      stream_putw (s, AFI_IP);	 
      stream_putc (s, SAFI_MPLS_LABELED_VPN);	 

      stream_putc (s, 12);
      stream_putl (s, 0);
      stream_putl (s, 0);
      stream_put (s, &attr->extra->mp_nexthop_global_in, 4);

       
      stream_putc (s, 0);

       
      stream_putc (s, p->prefixlen + 88);
      stream_put (s, tag, 3);
      stream_put (s, prd->val, 8);
      stream_put (s, &p->u.prefix, PSIZE (p->prefixlen));

       
      stream_putc_at (s, sizep, (stream_get_endp (s) - sizep) - 1);
    }

   
  if (CHECK_FLAG (peer->af_flags[afi][safi], PEER_FLAG_SEND_EXT_COMMUNITY) 
      && (attr->flag & ATTR_FLAG_BIT (BGP_ATTR_EXT_COMMUNITIES)))
    {
      struct attr_extra *attre = attr->extra;
      
      assert (attre);
      
      if (peer_sort (peer) == BGP_PEER_IBGP 
          || peer_sort (peer) == BGP_PEER_CONFED)
	{
	  if (attre->ecommunity->size * 8 > 255)
	    {
	      stream_putc (s, BGP_ATTR_FLAG_OPTIONAL|BGP_ATTR_FLAG_TRANS|BGP_ATTR_FLAG_EXTLEN);
	      stream_putc (s, BGP_ATTR_EXT_COMMUNITIES);
	      stream_putw (s, attre->ecommunity->size * 8);
	    }
	  else
	    {
	      stream_putc (s, BGP_ATTR_FLAG_OPTIONAL|BGP_ATTR_FLAG_TRANS);
	      stream_putc (s, BGP_ATTR_EXT_COMMUNITIES);
	      stream_putc (s, attre->ecommunity->size * 8);
	    }
	  stream_put (s, attre->ecommunity->val, attre->ecommunity->size * 8);
	}
      else
	{
	  u_int8_t *pnt;
	  int tbit;
	  int ecom_tr_size = 0;
	  int i;

	  for (i = 0; i < attre->ecommunity->size; i++)
	    {
	      pnt = attre->ecommunity->val + (i * 8);
	      tbit = *pnt;

	      if (CHECK_FLAG (tbit, ECOMMUNITY_FLAG_NON_TRANSITIVE))
		continue;

	      ecom_tr_size++;
	    }

	  if (ecom_tr_size)
	    {
	      if (ecom_tr_size * 8 > 255)
		{
		  stream_putc (s, BGP_ATTR_FLAG_OPTIONAL|BGP_ATTR_FLAG_TRANS|BGP_ATTR_FLAG_EXTLEN);
		  stream_putc (s, BGP_ATTR_EXT_COMMUNITIES);
		  stream_putw (s, ecom_tr_size * 8);
		}
	      else
		{
		  stream_putc (s, BGP_ATTR_FLAG_OPTIONAL|BGP_ATTR_FLAG_TRANS);
		  stream_putc (s, BGP_ATTR_EXT_COMMUNITIES);
		  stream_putc (s, ecom_tr_size * 8);
		}

	      for (i = 0; i < attre->ecommunity->size; i++)
		{
		  pnt = attre->ecommunity->val + (i * 8);
		  tbit = *pnt;

		  if (CHECK_FLAG (tbit, ECOMMUNITY_FLAG_NON_TRANSITIVE))
		    continue;

		  stream_put (s, pnt, 8);
		}
	    }
	}
    }

  if ( send_as4_path )
    {
       
       

       
      aspath = aspath_delete_confed_seq (aspath);

      stream_putc (s, BGP_ATTR_FLAG_TRANS|BGP_ATTR_FLAG_OPTIONAL|BGP_ATTR_FLAG_EXTLEN);
      stream_putc (s, BGP_ATTR_AS4_PATH);
      aspath_sizep = stream_get_endp (s);
      stream_putw (s, 0);
      stream_putw_at (s, aspath_sizep, aspath_put (s, aspath, 1));
    }

  if (aspath != attr->aspath)
    aspath_free (aspath);

  if ( send_as4_aggregator ) 
    {
      assert (attr->extra);

       
       
      stream_putc (s, BGP_ATTR_FLAG_OPTIONAL|BGP_ATTR_FLAG_TRANS);
      stream_putc (s, BGP_ATTR_AS4_AGGREGATOR);
      stream_putc (s, 8);
      stream_putl (s, attr->extra->aggregator_as);
      stream_put_ipv4 (s, attr->extra->aggregator_addr.s_addr);
    }
  
   
  if (attr->extra && attr->extra->transit)
    stream_put (s, attr->extra->transit->val, attr->extra->transit->length);

   
  return stream_get_endp (s) - cp;
}
