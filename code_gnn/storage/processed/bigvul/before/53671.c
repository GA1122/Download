int ipv6_recv_error(struct sock *sk, struct msghdr *msg, int len, int *addr_len)
{
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct sock_exterr_skb *serr;
	struct sk_buff *skb;
	DECLARE_SOCKADDR(struct sockaddr_in6 *, sin, msg->msg_name);
	struct {
		struct sock_extended_err ee;
		struct sockaddr_in6	 offender;
	} errhdr;
	int err;
	int copied;

	err = -EAGAIN;
	skb = sock_dequeue_err_skb(sk);
	if (!skb)
		goto out;

	copied = skb->len;
	if (copied > len) {
		msg->msg_flags |= MSG_TRUNC;
		copied = len;
	}
	err = skb_copy_datagram_msg(skb, 0, msg, copied);
	if (err)
		goto out_free_skb;

	sock_recv_timestamp(msg, sk, skb);

	serr = SKB_EXT_ERR(skb);

	if (sin && ipv6_datagram_support_addr(serr)) {
		const unsigned char *nh = skb_network_header(skb);
		sin->sin6_family = AF_INET6;
		sin->sin6_flowinfo = 0;
		sin->sin6_port = serr->port;
		if (skb->protocol == htons(ETH_P_IPV6)) {
			const struct ipv6hdr *ip6h = container_of((struct in6_addr *)(nh + serr->addr_offset),
								  struct ipv6hdr, daddr);
			sin->sin6_addr = ip6h->daddr;
			if (np->sndflow)
				sin->sin6_flowinfo = ip6_flowinfo(ip6h);
			sin->sin6_scope_id =
				ipv6_iface_scope_id(&sin->sin6_addr,
						    IP6CB(skb)->iif);
		} else {
			ipv6_addr_set_v4mapped(*(__be32 *)(nh + serr->addr_offset),
					       &sin->sin6_addr);
			sin->sin6_scope_id = 0;
		}
		*addr_len = sizeof(*sin);
	}

	memcpy(&errhdr.ee, &serr->ee, sizeof(struct sock_extended_err));
	sin = &errhdr.offender;
	memset(sin, 0, sizeof(*sin));

	if (ip6_datagram_support_cmsg(skb, serr)) {
		sin->sin6_family = AF_INET6;
		if (np->rxopt.all)
			ip6_datagram_recv_common_ctl(sk, msg, skb);
		if (skb->protocol == htons(ETH_P_IPV6)) {
			sin->sin6_addr = ipv6_hdr(skb)->saddr;
			if (np->rxopt.all)
				ip6_datagram_recv_specific_ctl(sk, msg, skb);
			sin->sin6_scope_id =
				ipv6_iface_scope_id(&sin->sin6_addr,
						    IP6CB(skb)->iif);
		} else {
			ipv6_addr_set_v4mapped(ip_hdr(skb)->saddr,
					       &sin->sin6_addr);
			if (inet_sk(sk)->cmsg_flags)
				ip_cmsg_recv(msg, skb);
		}
	}

	put_cmsg(msg, SOL_IPV6, IPV6_RECVERR, sizeof(errhdr), &errhdr);

	 

	msg->msg_flags |= MSG_ERRQUEUE;
	err = copied;

out_free_skb:
	kfree_skb(skb);
out:
	return err;
}