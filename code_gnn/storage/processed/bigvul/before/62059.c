struct rtable *ip_route_output_key_hash_rcu(struct net *net, struct flowi4 *fl4,
					    struct fib_result *res,
					    const struct sk_buff *skb)
{
	struct net_device *dev_out = NULL;
	int orig_oif = fl4->flowi4_oif;
	unsigned int flags = 0;
	struct rtable *rth;
	int err = -ENETUNREACH;

	if (fl4->saddr) {
		rth = ERR_PTR(-EINVAL);
		if (ipv4_is_multicast(fl4->saddr) ||
		    ipv4_is_lbcast(fl4->saddr) ||
		    ipv4_is_zeronet(fl4->saddr))
			goto out;

		 

		if (fl4->flowi4_oif == 0 &&
		    (ipv4_is_multicast(fl4->daddr) ||
		     ipv4_is_lbcast(fl4->daddr))) {
			 
			dev_out = __ip_dev_find(net, fl4->saddr, false);
			if (!dev_out)
				goto out;

			 

			fl4->flowi4_oif = dev_out->ifindex;
			goto make_route;
		}

		if (!(fl4->flowi4_flags & FLOWI_FLAG_ANYSRC)) {
			 
			if (!__ip_dev_find(net, fl4->saddr, false))
				goto out;
		}
	}


	if (fl4->flowi4_oif) {
		dev_out = dev_get_by_index_rcu(net, fl4->flowi4_oif);
		rth = ERR_PTR(-ENODEV);
		if (!dev_out)
			goto out;

		 
		if (!(dev_out->flags & IFF_UP) || !__in_dev_get_rcu(dev_out)) {
			rth = ERR_PTR(-ENETUNREACH);
			goto out;
		}
		if (ipv4_is_local_multicast(fl4->daddr) ||
		    ipv4_is_lbcast(fl4->daddr) ||
		    fl4->flowi4_proto == IPPROTO_IGMP) {
			if (!fl4->saddr)
				fl4->saddr = inet_select_addr(dev_out, 0,
							      RT_SCOPE_LINK);
			goto make_route;
		}
		if (!fl4->saddr) {
			if (ipv4_is_multicast(fl4->daddr))
				fl4->saddr = inet_select_addr(dev_out, 0,
							      fl4->flowi4_scope);
			else if (!fl4->daddr)
				fl4->saddr = inet_select_addr(dev_out, 0,
							      RT_SCOPE_HOST);
		}
	}

	if (!fl4->daddr) {
		fl4->daddr = fl4->saddr;
		if (!fl4->daddr)
			fl4->daddr = fl4->saddr = htonl(INADDR_LOOPBACK);
		dev_out = net->loopback_dev;
		fl4->flowi4_oif = LOOPBACK_IFINDEX;
		res->type = RTN_LOCAL;
		flags |= RTCF_LOCAL;
		goto make_route;
	}

	err = fib_lookup(net, fl4, res, 0);
	if (err) {
		res->fi = NULL;
		res->table = NULL;
		if (fl4->flowi4_oif &&
		    (ipv4_is_multicast(fl4->daddr) ||
		    !netif_index_is_l3_master(net, fl4->flowi4_oif))) {
			 

			if (fl4->saddr == 0)
				fl4->saddr = inet_select_addr(dev_out, 0,
							      RT_SCOPE_LINK);
			res->type = RTN_UNICAST;
			goto make_route;
		}
		rth = ERR_PTR(err);
		goto out;
	}

	if (res->type == RTN_LOCAL) {
		if (!fl4->saddr) {
			if (res->fi->fib_prefsrc)
				fl4->saddr = res->fi->fib_prefsrc;
			else
				fl4->saddr = fl4->daddr;
		}

		 
		dev_out = l3mdev_master_dev_rcu(FIB_RES_DEV(*res)) ? :
			net->loopback_dev;
		fl4->flowi4_oif = dev_out->ifindex;
		flags |= RTCF_LOCAL;
		goto make_route;
	}

	fib_select_path(net, res, fl4, skb);

	dev_out = FIB_RES_DEV(*res);
	fl4->flowi4_oif = dev_out->ifindex;


make_route:
	rth = __mkroute_output(res, fl4, orig_oif, dev_out, flags);

out:
	return rth;
}
