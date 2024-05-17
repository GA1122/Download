static int pppol2tp_sendmsg(struct kiocb *iocb, struct socket *sock, struct msghdr *m,
			    size_t total_len)
{
	static const unsigned char ppph[2] = { 0xff, 0x03 };
	struct sock *sk = sock->sk;
	struct sk_buff *skb;
	int error;
	struct l2tp_session *session;
	struct l2tp_tunnel *tunnel;
	struct pppol2tp_session *ps;
	int uhlen;

	error = -ENOTCONN;
	if (sock_flag(sk, SOCK_DEAD) || !(sk->sk_state & PPPOX_CONNECTED))
		goto error;

	 
	error = -EBADF;
	session = pppol2tp_sock_to_session(sk);
	if (session == NULL)
		goto error;

	ps = l2tp_session_priv(session);
	tunnel = l2tp_sock_to_tunnel(ps->tunnel_sock);
	if (tunnel == NULL)
		goto error_put_sess;

	uhlen = (tunnel->encap == L2TP_ENCAPTYPE_UDP) ? sizeof(struct udphdr) : 0;

	 
	error = -ENOMEM;
	skb = sock_wmalloc(sk, NET_SKB_PAD + sizeof(struct iphdr) +
			   uhlen + session->hdr_len +
			   sizeof(ppph) + total_len,
			   0, GFP_KERNEL);
	if (!skb)
		goto error_put_sess_tun;

	 
	skb_reserve(skb, NET_SKB_PAD);
	skb_reset_network_header(skb);
	skb_reserve(skb, sizeof(struct iphdr));
	skb_reset_transport_header(skb);
	skb_reserve(skb, uhlen);

	 
	skb->data[0] = ppph[0];
	skb->data[1] = ppph[1];
	skb_put(skb, 2);

	 
	error = memcpy_fromiovec(skb_put(skb, total_len), m->msg_iov,
				 total_len);
	if (error < 0) {
		kfree_skb(skb);
		goto error_put_sess_tun;
	}

	local_bh_disable();
	l2tp_xmit_skb(session, skb, session->hdr_len);
	local_bh_enable();

	sock_put(ps->tunnel_sock);
	sock_put(sk);

	return total_len;

error_put_sess_tun:
	sock_put(ps->tunnel_sock);
error_put_sess:
	sock_put(sk);
error:
	return error;
}