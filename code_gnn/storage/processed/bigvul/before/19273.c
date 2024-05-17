int nlmsg_notify(struct sock *sk, struct sk_buff *skb, u32 pid,
		 unsigned int group, int report, gfp_t flags)
{
	int err = 0;

	if (group) {
		int exclude_pid = 0;

		if (report) {
			atomic_inc(&skb->users);
			exclude_pid = pid;
		}

		 
		err = nlmsg_multicast(sk, skb, exclude_pid, group, flags);
	}

	if (report) {
		int err2;

		err2 = nlmsg_unicast(sk, skb, pid);
		if (!err || err == -ESRCH)
			err = err2;
	}

	return err;
}
