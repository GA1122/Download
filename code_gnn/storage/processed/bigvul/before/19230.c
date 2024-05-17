int netlink_dump_start(struct sock *ssk, struct sk_buff *skb,
		       const struct nlmsghdr *nlh,
		       struct netlink_dump_control *control)
{
	struct netlink_callback *cb;
	struct sock *sk;
	struct netlink_sock *nlk;
	int ret;

	cb = kzalloc(sizeof(*cb), GFP_KERNEL);
	if (cb == NULL)
		return -ENOBUFS;

	cb->dump = control->dump;
	cb->done = control->done;
	cb->nlh = nlh;
	cb->data = control->data;
	cb->min_dump_alloc = control->min_dump_alloc;
	atomic_inc(&skb->users);
	cb->skb = skb;

	sk = netlink_lookup(sock_net(ssk), ssk->sk_protocol, NETLINK_CB(skb).pid);
	if (sk == NULL) {
		netlink_destroy_callback(cb);
		return -ECONNREFUSED;
	}
	nlk = nlk_sk(sk);
	 
	mutex_lock(nlk->cb_mutex);
	if (nlk->cb) {
		mutex_unlock(nlk->cb_mutex);
		netlink_destroy_callback(cb);
		sock_put(sk);
		return -EBUSY;
	}
	nlk->cb = cb;
	mutex_unlock(nlk->cb_mutex);

	ret = netlink_dump(sk);

	sock_put(sk);

	if (ret)
		return ret;

	 
	return -EINTR;
}