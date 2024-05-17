static int netlink_autobind(struct socket *sock)
{
	struct sock *sk = sock->sk;
	struct net *net = sock_net(sk);
	struct nl_pid_hash *hash = &nl_table[sk->sk_protocol].hash;
	struct hlist_head *head;
	struct sock *osk;
	struct hlist_node *node;
	s32 pid = task_tgid_vnr(current);
	int err;
	static s32 rover = -4097;

retry:
	cond_resched();
	netlink_table_grab();
	head = nl_pid_hashfn(hash, pid);
	sk_for_each(osk, node, head) {
		if (!net_eq(sock_net(osk), net))
			continue;
		if (nlk_sk(osk)->pid == pid) {
			 
			pid = rover--;
			if (rover > -4097)
				rover = -4097;
			netlink_table_ungrab();
			goto retry;
		}
	}
	netlink_table_ungrab();

	err = netlink_insert(sk, net, pid);
	if (err == -EADDRINUSE)
		goto retry;

	 
	if (err == -EBUSY)
		err = 0;

	return err;
}