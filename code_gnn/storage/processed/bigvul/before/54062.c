static int devinet_sysctl_forward(struct ctl_table *ctl, int write,
				  void __user *buffer,
				  size_t *lenp, loff_t *ppos)
{
	int *valp = ctl->data;
	int val = *valp;
	loff_t pos = *ppos;
	int ret = proc_dointvec(ctl, write, buffer, lenp, ppos);

	if (write && *valp != val) {
		struct net *net = ctl->extra2;

		if (valp != &IPV4_DEVCONF_DFLT(net, FORWARDING)) {
			if (!rtnl_trylock()) {
				 
				*valp = val;
				*ppos = pos;
				return restart_syscall();
			}
			if (valp == &IPV4_DEVCONF_ALL(net, FORWARDING)) {
				inet_forward_change(net);
			} else {
				struct ipv4_devconf *cnf = ctl->extra1;
				struct in_device *idev =
					container_of(cnf, struct in_device, cnf);
				if (*valp)
					dev_disable_lro(idev->dev);
				inet_netconf_notify_devconf(net,
							    NETCONFA_FORWARDING,
							    idev->dev->ifindex,
							    cnf);
			}
			rtnl_unlock();
			rt_cache_flush(net);
		} else
			inet_netconf_notify_devconf(net, NETCONFA_FORWARDING,
						    NETCONFA_IFINDEX_DEFAULT,
						    net->ipv4.devconf_dflt);
	}

	return ret;
}