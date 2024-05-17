static int mif6_add(struct net *net, struct mr6_table *mrt,
		    struct mif6ctl *vifc, int mrtsock)
{
	int vifi = vifc->mif6c_mifi;
	struct mif_device *v = &mrt->vif6_table[vifi];
	struct net_device *dev;
	struct inet6_dev *in6_dev;
	int err;

	 
	if (MIF_EXISTS(mrt, vifi))
		return -EADDRINUSE;

	switch (vifc->mif6c_flags) {
#ifdef CONFIG_IPV6_PIMSM_V2
	case MIFF_REGISTER:
		 
		if (mrt->mroute_reg_vif_num >= 0)
			return -EADDRINUSE;
		dev = ip6mr_reg_vif(net, mrt);
		if (!dev)
			return -ENOBUFS;
		err = dev_set_allmulti(dev, 1);
		if (err) {
			unregister_netdevice(dev);
			dev_put(dev);
			return err;
		}
		break;
#endif
	case 0:
		dev = dev_get_by_index(net, vifc->mif6c_pifi);
		if (!dev)
			return -EADDRNOTAVAIL;
		err = dev_set_allmulti(dev, 1);
		if (err) {
			dev_put(dev);
			return err;
		}
		break;
	default:
		return -EINVAL;
	}

	in6_dev = __in6_dev_get(dev);
	if (in6_dev) {
		in6_dev->cnf.mc_forwarding++;
		inet6_netconf_notify_devconf(dev_net(dev),
					     NETCONFA_MC_FORWARDING,
					     dev->ifindex, &in6_dev->cnf);
	}

	 
	v->rate_limit = vifc->vifc_rate_limit;
	v->flags = vifc->mif6c_flags;
	if (!mrtsock)
		v->flags |= VIFF_STATIC;
	v->threshold = vifc->vifc_threshold;
	v->bytes_in = 0;
	v->bytes_out = 0;
	v->pkt_in = 0;
	v->pkt_out = 0;
	v->link = dev->ifindex;
	if (v->flags & MIFF_REGISTER)
		v->link = dev_get_iflink(dev);

	 
	write_lock_bh(&mrt_lock);
	v->dev = dev;
#ifdef CONFIG_IPV6_PIMSM_V2
	if (v->flags & MIFF_REGISTER)
		mrt->mroute_reg_vif_num = vifi;
#endif
	if (vifi + 1 > mrt->maxvif)
		mrt->maxvif = vifi + 1;
	write_unlock_bh(&mrt_lock);
	return 0;
}