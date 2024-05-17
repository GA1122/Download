rtadv_send_packet (int sock, struct interface *ifp)
{
  struct msghdr msg;
  struct iovec iov;
  struct cmsghdr  *cmsgptr;
  struct in6_pktinfo *pkt;
  struct sockaddr_in6 addr;
#ifdef HAVE_STRUCT_SOCKADDR_DL
  struct sockaddr_dl *sdl;
#endif  
  static void *adata = NULL;
  unsigned char buf[RTADV_MSG_SIZE];
  struct nd_router_advert *rtadv;
  int ret;
  int len = 0;
  struct zebra_if *zif;
  struct rtadv_prefix *rprefix;
  u_char all_nodes_addr[] = {0xff,0x02,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
  struct listnode *node;
  u_int16_t pkt_RouterLifetime;

   
  if (adata == NULL)
    {
       
      adata = malloc(CMSG_SPACE(sizeof(struct in6_pktinfo)));
	   
      if (adata == NULL)
	zlog_err("rtadv_send_packet: can't malloc control data\n");
    }

   
  if (IS_ZEBRA_DEBUG_PACKET)
    zlog_debug ("Router advertisement send to %s", ifp->name);

   
  memset (&addr, 0, sizeof (struct sockaddr_in6));
  addr.sin6_family = AF_INET6;
#ifdef SIN6_LEN
  addr.sin6_len = sizeof (struct sockaddr_in6);
#endif  
  addr.sin6_port = htons (IPPROTO_ICMPV6);
  IPV6_ADDR_COPY (&addr.sin6_addr, all_nodes_addr);

   
  zif = ifp->info;

   
  rtadv = (struct nd_router_advert *) buf;

  rtadv->nd_ra_type = ND_ROUTER_ADVERT;
  rtadv->nd_ra_code = 0;
  rtadv->nd_ra_cksum = 0;

  rtadv->nd_ra_curhoplimit = 64;

   
  rtadv->nd_ra_flags_reserved =
    zif->rtadv.AdvDefaultLifetime == 0 ? 0 : zif->rtadv.DefaultPreference;
  rtadv->nd_ra_flags_reserved <<= 3;

  if (zif->rtadv.AdvManagedFlag)
    rtadv->nd_ra_flags_reserved |= ND_RA_FLAG_MANAGED;
  if (zif->rtadv.AdvOtherConfigFlag)
    rtadv->nd_ra_flags_reserved |= ND_RA_FLAG_OTHER;
  if (zif->rtadv.AdvHomeAgentFlag)
    rtadv->nd_ra_flags_reserved |= ND_RA_FLAG_HOME_AGENT;
   
  pkt_RouterLifetime = zif->rtadv.AdvDefaultLifetime != -1 ?
    zif->rtadv.AdvDefaultLifetime :
    MAX (1, 0.003 * zif->rtadv.MaxRtrAdvInterval);
  rtadv->nd_ra_router_lifetime = htons (pkt_RouterLifetime);
  rtadv->nd_ra_reachable = htonl (zif->rtadv.AdvReachableTime);
  rtadv->nd_ra_retransmit = htonl (0);

  len = sizeof (struct nd_router_advert);

   
  if
  (
    zif->rtadv.AdvHomeAgentFlag &&
    (zif->rtadv.HomeAgentPreference || zif->rtadv.HomeAgentLifetime != -1)
  )
    {
      struct nd_opt_homeagent_info *ndopt_hai = 
	(struct nd_opt_homeagent_info *)(buf + len);
      ndopt_hai->nd_opt_hai_type = ND_OPT_HA_INFORMATION;
      ndopt_hai->nd_opt_hai_len = 1;
      ndopt_hai->nd_opt_hai_reserved = 0;
      ndopt_hai->nd_opt_hai_preference = htons(zif->rtadv.HomeAgentPreference);
       
      ndopt_hai->nd_opt_hai_lifetime = htons
      (
        zif->rtadv.HomeAgentLifetime != -1 ?
        zif->rtadv.HomeAgentLifetime :
        MAX (1, pkt_RouterLifetime)  
      );
      len += sizeof(struct nd_opt_homeagent_info);
    }

  if (zif->rtadv.AdvIntervalOption)
    {
      struct nd_opt_adv_interval *ndopt_adv = 
	(struct nd_opt_adv_interval *)(buf + len);
      ndopt_adv->nd_opt_ai_type = ND_OPT_ADV_INTERVAL;
      ndopt_adv->nd_opt_ai_len = 1;
      ndopt_adv->nd_opt_ai_reserved = 0;
      ndopt_adv->nd_opt_ai_interval = htonl(zif->rtadv.MaxRtrAdvInterval);
      len += sizeof(struct nd_opt_adv_interval);
    }

   
  for (ALL_LIST_ELEMENTS_RO (zif->rtadv.AdvPrefixList, node, rprefix))
    {
      struct nd_opt_prefix_info *pinfo;

      pinfo = (struct nd_opt_prefix_info *) (buf + len);

      pinfo->nd_opt_pi_type = ND_OPT_PREFIX_INFORMATION;
      pinfo->nd_opt_pi_len = 4;
      pinfo->nd_opt_pi_prefix_len = rprefix->prefix.prefixlen;

      pinfo->nd_opt_pi_flags_reserved = 0;
      if (rprefix->AdvOnLinkFlag)
	pinfo->nd_opt_pi_flags_reserved |= ND_OPT_PI_FLAG_ONLINK;
      if (rprefix->AdvAutonomousFlag)
	pinfo->nd_opt_pi_flags_reserved |= ND_OPT_PI_FLAG_AUTO;
      if (rprefix->AdvRouterAddressFlag)
	pinfo->nd_opt_pi_flags_reserved |= ND_OPT_PI_FLAG_RADDR;

      pinfo->nd_opt_pi_valid_time = htonl (rprefix->AdvValidLifetime);
      pinfo->nd_opt_pi_preferred_time = htonl (rprefix->AdvPreferredLifetime);
      pinfo->nd_opt_pi_reserved2 = 0;

      IPV6_ADDR_COPY (&pinfo->nd_opt_pi_prefix, &rprefix->prefix.prefix);

#ifdef DEBUG
      {
	u_char buf[INET6_ADDRSTRLEN];

	zlog_debug ("DEBUG %s", inet_ntop (AF_INET6, &pinfo->nd_opt_pi_prefix, 
	           buf, INET6_ADDRSTRLEN));

      }
#endif  

      len += sizeof (struct nd_opt_prefix_info);
    }

   
  if (ifp->hw_addr_len != 0)
    {
      buf[len++] = ND_OPT_SOURCE_LINKADDR;

       
      buf[len++] = (ifp->hw_addr_len + 9) >> 3;

      memcpy (buf + len, ifp->hw_addr, ifp->hw_addr_len);
      len += ifp->hw_addr_len;

       
      memset (buf + len, 0, -(ifp->hw_addr_len + 2) & 0x7);
      len += -(ifp->hw_addr_len + 2) & 0x7;
    }

   
  if (zif->rtadv.AdvLinkMTU)
    {
      struct nd_opt_mtu * opt = (struct nd_opt_mtu *) (buf + len);
      opt->nd_opt_mtu_type = ND_OPT_MTU;
      opt->nd_opt_mtu_len = 1;
      opt->nd_opt_mtu_reserved = 0;
      opt->nd_opt_mtu_mtu = htonl (zif->rtadv.AdvLinkMTU);
      len += sizeof (struct nd_opt_mtu);
    }

  msg.msg_name = (void *) &addr;
  msg.msg_namelen = sizeof (struct sockaddr_in6);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = (void *) adata;
  msg.msg_controllen = CMSG_SPACE(sizeof(struct in6_pktinfo));
  msg.msg_flags = 0;
  iov.iov_base = buf;
  iov.iov_len = len;

  cmsgptr = ZCMSG_FIRSTHDR(&msg);
  cmsgptr->cmsg_len = CMSG_LEN(sizeof(struct in6_pktinfo));
  cmsgptr->cmsg_level = IPPROTO_IPV6;
  cmsgptr->cmsg_type = IPV6_PKTINFO;

  pkt = (struct in6_pktinfo *) CMSG_DATA (cmsgptr);
  memset (&pkt->ipi6_addr, 0, sizeof (struct in6_addr));
  pkt->ipi6_ifindex = ifp->ifindex;

  ret = sendmsg (sock, &msg, 0);
  if (ret < 0)
    {
      zlog_err ("rtadv_send_packet: sendmsg %d (%s)\n",
		errno, safe_strerror(errno));
    }
}
