int ip_rt_ioctl(struct net *net, unsigned int cmd, void __user *arg)
{
	struct fib_config cfg;
	struct rtentry rt;
	int err;

	switch (cmd) {
	case SIOCADDRT:		 
	case SIOCDELRT:		 
		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			return -EPERM;

		if (copy_from_user(&rt, arg, sizeof(rt)))
			return -EFAULT;

		rtnl_lock();
		err = rtentry_to_fib_config(net, cmd, &rt, &cfg);
		if (err == 0) {
			struct fib_table *tb;

			if (cmd == SIOCDELRT) {
				tb = fib_get_table(net, cfg.fc_table);
				if (tb)
					err = fib_table_delete(tb, &cfg);
				else
					err = -ESRCH;
			} else {
				tb = fib_new_table(net, cfg.fc_table);
				if (tb)
					err = fib_table_insert(tb, &cfg);
				else
					err = -ENOBUFS;
			}

			 
			kfree(cfg.fc_mx);
		}
		rtnl_unlock();
		return err;
	}
	return -EINVAL;
}