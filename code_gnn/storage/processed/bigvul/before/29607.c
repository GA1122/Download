static int ip_setup_cork(struct sock *sk, struct inet_cork *cork,
			 struct ipcm_cookie *ipc, struct rtable **rtp)
{
	struct inet_sock *inet = inet_sk(sk);
	struct ip_options_rcu *opt;
	struct rtable *rt;

	 
	opt = ipc->opt;
	if (opt) {
		if (cork->opt == NULL) {
			cork->opt = kmalloc(sizeof(struct ip_options) + 40,
					    sk->sk_allocation);
			if (unlikely(cork->opt == NULL))
				return -ENOBUFS;
		}
		memcpy(cork->opt, &opt->opt, sizeof(struct ip_options) + opt->opt.optlen);
		cork->flags |= IPCORK_OPT;
		cork->addr = ipc->addr;
	}
	rt = *rtp;
	if (unlikely(!rt))
		return -EFAULT;
	 
	*rtp = NULL;
	cork->fragsize = inet->pmtudisc == IP_PMTUDISC_PROBE ?
			 rt->dst.dev->mtu : dst_mtu(&rt->dst);
	cork->dst = &rt->dst;
	cork->length = 0;
	cork->tx_flags = ipc->tx_flags;

	return 0;
}