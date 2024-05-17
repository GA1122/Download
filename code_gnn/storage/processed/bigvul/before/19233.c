static struct sock *netlink_getsockbypid(struct sock *ssk, u32 pid)
{
	struct sock *sock;
	struct netlink_sock *nlk;

	sock = netlink_lookup(sock_net(ssk), ssk->sk_protocol, pid);
	if (!sock)
		return ERR_PTR(-ECONNREFUSED);

	 
	nlk = nlk_sk(sock);
	if (sock->sk_state == NETLINK_CONNECTED &&
	    nlk->dst_pid != nlk_sk(ssk)->pid) {
		sock_put(sock);
		return ERR_PTR(-ECONNREFUSED);
	}
	return sock;
}