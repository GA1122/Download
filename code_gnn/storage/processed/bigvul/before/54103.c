static struct in_device *inetdev_init(struct net_device *dev)
{
	struct in_device *in_dev;
	int err = -ENOMEM;

	ASSERT_RTNL();

	in_dev = kzalloc(sizeof(*in_dev), GFP_KERNEL);
	if (!in_dev)
		goto out;
	memcpy(&in_dev->cnf, dev_net(dev)->ipv4.devconf_dflt,
			sizeof(in_dev->cnf));
	in_dev->cnf.sysctl = NULL;
	in_dev->dev = dev;
	in_dev->arp_parms = neigh_parms_alloc(dev, &arp_tbl);
	if (!in_dev->arp_parms)
		goto out_kfree;
	if (IPV4_DEVCONF(in_dev->cnf, FORWARDING))
		dev_disable_lro(dev);
	 
	dev_hold(dev);
	 
	in_dev_hold(in_dev);

	err = devinet_sysctl_register(in_dev);
	if (err) {
		in_dev->dead = 1;
		in_dev_put(in_dev);
		in_dev = NULL;
		goto out;
	}
	ip_mc_init_dev(in_dev);
	if (dev->flags & IFF_UP)
		ip_mc_up(in_dev);

	 
	rcu_assign_pointer(dev->ip_ptr, in_dev);
out:
	return in_dev ?: ERR_PTR(err);
out_kfree:
	kfree(in_dev);
	in_dev = NULL;
	goto out;
}