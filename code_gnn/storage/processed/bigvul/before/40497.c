int __netlink_dump_start(struct sock *ssk, struct sk_buff *skb,
			 const struct nlmsghdr *nlh,
			 struct netlink_dump_control *control)
{
	struct netlink_callback *cb;
	struct sock *sk;
	struct netlink_sock *nlk;
	int ret;

	 
	if (netlink_skb_is_mmaped(skb)) {
		skb = skb_copy(skb, GFP_KERNEL);
		if (skb == NULL)
			return -ENOBUFS;
	} else
		atomic_inc(&skb->users);

	sk = netlink_lookup(sock_net(ssk), ssk->sk_protocol, NETLINK_CB(skb).portid);
	if (sk == NULL) {
		ret = -ECONNREFUSED;
		goto error_free;
	}

	nlk = nlk_sk(sk);
	mutex_lock(nlk->cb_mutex);
	 
	if (nlk->cb_running) {
		ret = -EBUSY;
		goto error_unlock;
	}
	 
	if (!try_module_get(control->module)) {
		ret = -EPROTONOSUPPORT;
		goto error_unlock;
	}

	cb = &nlk->cb;
	memset(cb, 0, sizeof(*cb));
	cb->dump = control->dump;
	cb->done = control->done;
	cb->nlh = nlh;
	cb->data = control->data;
	cb->module = control->module;
	cb->min_dump_alloc = control->min_dump_alloc;
	cb->skb = skb;

	nlk->cb_running = true;

	mutex_unlock(nlk->cb_mutex);

	ret = netlink_dump(sk);
	sock_put(sk);

	if (ret)
		return ret;

	 
	return -EINTR;

error_unlock:
	sock_put(sk);
	mutex_unlock(nlk->cb_mutex);
error_free:
	kfree_skb(skb);
	return ret;
}