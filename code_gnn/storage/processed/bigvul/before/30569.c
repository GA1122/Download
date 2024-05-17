static int l2tp_ip6_recv(struct sk_buff *skb)
{
	struct sock *sk;
	u32 session_id;
	u32 tunnel_id;
	unsigned char *ptr, *optr;
	struct l2tp_session *session;
	struct l2tp_tunnel *tunnel = NULL;
	int length;

	 
	optr = ptr = skb->data;

	if (!pskb_may_pull(skb, 4))
		goto discard;

	session_id = ntohl(*((__be32 *) ptr));
	ptr += 4;

	 
	if (session_id == 0) {
		__skb_pull(skb, 4);
		goto pass_up;
	}

	 
	session = l2tp_session_find(&init_net, NULL, session_id);
	if (session == NULL)
		goto discard;

	tunnel = session->tunnel;
	if (tunnel == NULL)
		goto discard;

	 
	if (tunnel->debug & L2TP_MSG_DATA) {
		length = min(32u, skb->len);
		if (!pskb_may_pull(skb, length))
			goto discard;

		pr_debug("%s: ip recv\n", tunnel->name);
		print_hex_dump_bytes("", DUMP_PREFIX_OFFSET, ptr, length);
	}

	l2tp_recv_common(session, skb, ptr, optr, 0, skb->len,
			 tunnel->recv_payload_hook);
	return 0;

pass_up:
	 
	if (!pskb_may_pull(skb, 12))
		goto discard;

	if ((skb->data[0] & 0xc0) != 0xc0)
		goto discard;

	tunnel_id = ntohl(*(__be32 *) &skb->data[4]);
	tunnel = l2tp_tunnel_find(&init_net, tunnel_id);
	if (tunnel != NULL)
		sk = tunnel->sock;
	else {
		struct ipv6hdr *iph = ipv6_hdr(skb);

		read_lock_bh(&l2tp_ip6_lock);
		sk = __l2tp_ip6_bind_lookup(&init_net, &iph->daddr,
					    0, tunnel_id);
		read_unlock_bh(&l2tp_ip6_lock);
	}

	if (sk == NULL)
		goto discard;

	sock_hold(sk);

	if (!xfrm6_policy_check(sk, XFRM_POLICY_IN, skb))
		goto discard_put;

	nf_reset(skb);

	return sk_receive_skb(sk, skb, 1);

discard_put:
	sock_put(sk);

discard:
	kfree_skb(skb);
	return 0;
}