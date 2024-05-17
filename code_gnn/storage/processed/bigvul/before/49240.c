bool tcp_add_backlog(struct sock *sk, struct sk_buff *skb)
{
	u32 limit = sk->sk_rcvbuf + sk->sk_sndbuf;

	 
	limit += 64*1024;

	 
	if (!skb->data_len)
		skb->truesize = SKB_TRUESIZE(skb_end_offset(skb));

	if (unlikely(sk_add_backlog(sk, skb, limit))) {
		bh_unlock_sock(sk);
		__NET_INC_STATS(sock_net(sk), LINUX_MIB_TCPBACKLOGDROP);
		return true;
	}
	return false;
 }
