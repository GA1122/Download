static int udp_send_skb(struct sk_buff *skb, __be32 daddr, __be32 dport)
{
	struct sock *sk = skb->sk;
	struct inet_sock *inet = inet_sk(sk);
	struct udphdr *uh;
	struct rtable *rt = (struct rtable *)skb_dst(skb);
	int err = 0;
	int is_udplite = IS_UDPLITE(sk);
	int offset = skb_transport_offset(skb);
	int len = skb->len - offset;
	__wsum csum = 0;

	 
	uh = udp_hdr(skb);
	uh->source = inet->inet_sport;
	uh->dest = dport;
	uh->len = htons(len);
	uh->check = 0;

	if (is_udplite)  				  
		csum = udplite_csum(skb);

	else if (sk->sk_no_check == UDP_CSUM_NOXMIT) {    

		skb->ip_summed = CHECKSUM_NONE;
		goto send;

	} else if (skb->ip_summed == CHECKSUM_PARTIAL) {  

		udp4_hwcsum(skb, rt->rt_src, daddr);
		goto send;

	} else
		csum = udp_csum(skb);

	 
	uh->check = csum_tcpudp_magic(rt->rt_src, daddr, len,
				      sk->sk_protocol, csum);
	if (uh->check == 0)
		uh->check = CSUM_MANGLED_0;

send:
	err = ip_send_skb(skb);
	if (err) {
		if (err == -ENOBUFS && !inet->recverr) {
			UDP_INC_STATS_USER(sock_net(sk),
					   UDP_MIB_SNDBUFERRORS, is_udplite);
			err = 0;
		}
	} else
		UDP_INC_STATS_USER(sock_net(sk),
				   UDP_MIB_OUTDATAGRAMS, is_udplite);
	return err;
}