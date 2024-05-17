rtadv_process_packet (u_char *buf, unsigned int len, ifindex_t ifindex,
    int hoplimit, vrf_id_t vrf_id)
{
  struct icmp6_hdr *icmph;
  struct interface *ifp;
  struct zebra_if *zif;

   
  ifp = if_lookup_by_index_vrf (ifindex, vrf_id);
  if (ifp == NULL)
    {
      zlog_warn ("Unknown interface index: %d, vrf %u", ifindex, vrf_id);
      return;
    }

  if (if_is_loopback (ifp))
    return;

   
  zif = ifp->info;
  if (! zif->rtadv.AdvSendAdvertisements)
    return;

   
  if (len < sizeof (struct icmp6_hdr))
    {
      zlog_warn ("Invalid ICMPV6 packet length: %d", len);
      return;
    }

  icmph = (struct icmp6_hdr *) buf;

   
  if (icmph->icmp6_type != ND_ROUTER_SOLICIT &&
      icmph->icmp6_type != ND_ROUTER_ADVERT)
    {
      zlog_warn ("Unwanted ICMPV6 message type: %d", icmph->icmp6_type);
      return;
    }

   
  if (hoplimit >= 0 && hoplimit != 255)
    {
      zlog_warn ("Invalid hoplimit %d for router advertisement ICMP packet",
		 hoplimit);
      return;
    }

   
  if (icmph->icmp6_type == ND_ROUTER_SOLICIT)
    rtadv_process_solicit (ifp);
  else if (icmph->icmp6_type == ND_ROUTER_ADVERT)
    rtadv_process_advert ();

  return;
}
