 void icmp_send(struct sk_buff *skb_in, int type, int code, __be32 info)
 {
 	struct iphdr *iph;
 	int room;
 	struct icmp_bxm icmp_param;
 	struct rtable *rt = skb_rtable(skb_in);
 	struct ipcm_cookie ipc;
 	__be32 saddr;
 	u8  tos;
 	struct net *net;
 	struct sock *sk;
 
 	if (!rt)
 		goto out;
 	net = dev_net(rt->dst.dev);
 
 	 
 	iph = ip_hdr(skb_in);
 
 	if ((u8 *)iph < skb_in->head ||
 	    (skb_in->network_header + sizeof(*iph)) > skb_in->tail)
 		goto out;
 
 	 
 	if (skb_in->pkt_type != PACKET_HOST)
 		goto out;
 
 	 
 	if (rt->rt_flags & (RTCF_BROADCAST | RTCF_MULTICAST))
 		goto out;
 
 	 
 	if (iph->frag_off & htons(IP_OFFSET))
 		goto out;
 
 	 
 	if (icmp_pointers[type].error) {
 		 
 		if (iph->protocol == IPPROTO_ICMP) {
 			u8 _inner_type, *itp;
 
 			itp = skb_header_pointer(skb_in,
 						 skb_network_header(skb_in) +
 						 (iph->ihl << 2) +
 						 offsetof(struct icmphdr,
 							  type) -
 						 skb_in->data,
 						 sizeof(_inner_type),
 						 &_inner_type);
 			if (itp == NULL)
 				goto out;
 
 			 
 			if (*itp > NR_ICMP_TYPES ||
 			    icmp_pointers[*itp].error)
 				goto out;
 		}
 	}
 
 	sk = icmp_xmit_lock(net);
 	if (sk == NULL)
 		return;
 
 	 
 
 	saddr = iph->daddr;
 	if (!(rt->rt_flags & RTCF_LOCAL)) {
 		struct net_device *dev = NULL;
 
 		rcu_read_lock();
 		if (rt_is_input_route(rt) &&
 		    net->ipv4.sysctl_icmp_errors_use_inbound_ifaddr)
 			dev = dev_get_by_index_rcu(net, rt->rt_iif);
 
 		if (dev)
 			saddr = inet_select_addr(dev, 0, RT_SCOPE_LINK);
 		else
 			saddr = 0;
 		rcu_read_unlock();
 	}
 
 	tos = icmp_pointers[type].error ? ((iph->tos & IPTOS_TOS_MASK) |
  					   IPTOS_PREC_INTERNETCONTROL) :
  					  iph->tos;
  
	if (ip_options_echo(&icmp_param.replyopts, skb_in))
// 	if (ip_options_echo(&icmp_param.replyopts.opt.opt, skb_in))
  		goto out_unlock;
  
  
 	 
 
 	icmp_param.data.icmph.type	 = type;
 	icmp_param.data.icmph.code	 = code;
 	icmp_param.data.icmph.un.gateway = info;
 	icmp_param.data.icmph.checksum	 = 0;
 	icmp_param.skb	  = skb_in;
  	icmp_param.offset = skb_network_offset(skb_in);
  	inet_sk(sk)->tos = tos;
  	ipc.addr = iph->saddr;
	ipc.opt = &icmp_param.replyopts;
// 	ipc.opt = &icmp_param.replyopts.opt;
  	ipc.tx_flags = 0;
  
  	rt = icmp_route_lookup(net, skb_in, iph, saddr, tos,
 			       type, code, &icmp_param);
 	if (IS_ERR(rt))
 		goto out_unlock;
 
 	if (!icmpv4_xrlim_allow(net, rt, type, code))
 		goto ende;
 
 	 
 
  	room = dst_mtu(&rt->dst);
  	if (room > 576)
  		room = 576;
	room -= sizeof(struct iphdr) + icmp_param.replyopts.optlen;
// 	room -= sizeof(struct iphdr) + icmp_param.replyopts.opt.opt.optlen;
  	room -= sizeof(struct icmphdr);
  
  	icmp_param.data_len = skb_in->len - icmp_param.offset;
 	if (icmp_param.data_len > room)
 		icmp_param.data_len = room;
 	icmp_param.head_len = sizeof(struct icmphdr);
 
 	icmp_push_reply(&icmp_param, &ipc, &rt);
 ende:
 	ip_rt_put(rt);
 out_unlock:
 	icmp_xmit_unlock(sk);
 out:;
 }