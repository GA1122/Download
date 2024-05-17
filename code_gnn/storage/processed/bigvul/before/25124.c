void ip_rt_redirect(__be32 old_gw, __be32 daddr, __be32 new_gw,
		    __be32 saddr, struct net_device *dev)
{
	struct in_device *in_dev = __in_dev_get_rcu(dev);
	struct inet_peer *peer;
	struct net *net;

	if (!in_dev)
		return;

	net = dev_net(dev);
	if (new_gw == old_gw || !IN_DEV_RX_REDIRECTS(in_dev) ||
	    ipv4_is_multicast(new_gw) || ipv4_is_lbcast(new_gw) ||
	    ipv4_is_zeronet(new_gw))
		goto reject_redirect;

	if (!IN_DEV_SHARED_MEDIA(in_dev)) {
		if (!inet_addr_onlink(in_dev, new_gw, old_gw))
			goto reject_redirect;
		if (IN_DEV_SEC_REDIRECTS(in_dev) && ip_fib_check_default(new_gw, dev))
			goto reject_redirect;
	} else {
		if (inet_addr_type(net, new_gw) != RTN_UNICAST)
			goto reject_redirect;
	}

	peer = inet_getpeer_v4(daddr, 1);
	if (peer) {
		peer->redirect_learned.a4 = new_gw;

		inet_putpeer(peer);

		atomic_inc(&__rt_peer_genid);
	}
	return;

reject_redirect:
#ifdef CONFIG_IP_ROUTE_VERBOSE
	if (IN_DEV_LOG_MARTIANS(in_dev) && net_ratelimit())
		printk(KERN_INFO "Redirect from %pI4 on %s about %pI4 ignored.\n"
			"  Advised path = %pI4 -> %pI4\n",
		       &old_gw, dev->name, &new_gw,
		       &saddr, &daddr);
#endif
	;
}
