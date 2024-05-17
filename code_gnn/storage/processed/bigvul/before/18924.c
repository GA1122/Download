void ip_cmsg_recv(struct msghdr *msg, struct sk_buff *skb)
{
	struct inet_sock *inet = inet_sk(skb->sk);
	unsigned flags = inet->cmsg_flags;

	 
	if (flags & 1)
		ip_cmsg_recv_pktinfo(msg, skb);
	if ((flags >>= 1) == 0)
		return;

	if (flags & 1)
		ip_cmsg_recv_ttl(msg, skb);
	if ((flags >>= 1) == 0)
		return;

	if (flags & 1)
		ip_cmsg_recv_tos(msg, skb);
	if ((flags >>= 1) == 0)
		return;

	if (flags & 1)
		ip_cmsg_recv_opts(msg, skb);
	if ((flags >>= 1) == 0)
		return;

	if (flags & 1)
		ip_cmsg_recv_retopts(msg, skb);
	if ((flags >>= 1) == 0)
		return;

	if (flags & 1)
		ip_cmsg_recv_security(msg, skb);

	if ((flags >>= 1) == 0)
		return;
	if (flags & 1)
		ip_cmsg_recv_dstaddr(msg, skb);

}