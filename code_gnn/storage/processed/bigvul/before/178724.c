 static void icmp_reply(struct icmp_bxm *icmp_param, struct sk_buff *skb)
 {
 	struct ipcm_cookie ipc;
 	struct rtable *rt = skb_rtable(skb);
 	struct net *net = dev_net(rt->dst.dev);
 	struct sock *sk;
  	struct inet_sock *inet;
  	__be32 daddr;
  
	if (ip_options_echo(&icmp_param->replyopts, skb))
// 	if (ip_options_echo(&icmp_param->replyopts.opt.opt, skb))
  		return;
  
  	sk = icmp_xmit_lock(net);
 	if (sk == NULL)
 		return;
 	inet = inet_sk(sk);
 
 	icmp_param->data.icmph.checksum = 0;
 
 	inet->tos = ip_hdr(skb)->tos;
  	daddr = ipc.addr = rt->rt_src;
  	ipc.opt = NULL;
  	ipc.tx_flags = 0;
	if (icmp_param->replyopts.optlen) {
		ipc.opt = &icmp_param->replyopts;
		if (ipc.opt->srr)
			daddr = icmp_param->replyopts.faddr;
// 	if (icmp_param->replyopts.opt.opt.optlen) {
// 		ipc.opt = &icmp_param->replyopts.opt;
// 		if (ipc.opt->opt.srr)
// 			daddr = icmp_param->replyopts.opt.opt.faddr;
  	}
  	{
  		struct flowi4 fl4 = {
 			.daddr = daddr,
 			.saddr = rt->rt_spec_dst,
 			.flowi4_tos = RT_TOS(ip_hdr(skb)->tos),
 			.flowi4_proto = IPPROTO_ICMP,
 		};
 		security_skb_classify_flow(skb, flowi4_to_flowi(&fl4));
 		rt = ip_route_output_key(net, &fl4);
 		if (IS_ERR(rt))
 			goto out_unlock;
 	}
 	if (icmpv4_xrlim_allow(net, rt, icmp_param->data.icmph.type,
 			       icmp_param->data.icmph.code))
 		icmp_push_reply(icmp_param, &ipc, &rt);
 	ip_rt_put(rt);
 out_unlock:
 	icmp_xmit_unlock(sk);
 }