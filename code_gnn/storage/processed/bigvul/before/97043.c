static int ip6_dst_lookup_tail(struct sock *sk,
			       struct dst_entry **dst, struct flowi6 *fl6)
{
	struct net *net = sock_net(sk);
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	struct neighbour *n;
	struct rt6_info *rt;
#endif
	int err;

	if (*dst == NULL)
		*dst = ip6_route_output(net, sk, fl6);

	if ((err = (*dst)->error))
		goto out_err_release;

	if (ipv6_addr_any(&fl6->saddr)) {
		struct rt6_info *rt = (struct rt6_info *) *dst;
		err = ip6_route_get_saddr(net, rt, &fl6->daddr,
					  sk ? inet6_sk(sk)->srcprefs : 0,
					  &fl6->saddr);
		if (err)
			goto out_err_release;
	}

#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	 
	rt = (struct rt6_info *) *dst;
	rcu_read_lock_bh();
	n = __ipv6_neigh_lookup_noref(rt->dst.dev, rt6_nexthop(rt));
	err = n && !(n->nud_state & NUD_VALID) ? -EINVAL : 0;
	rcu_read_unlock_bh();

	if (err) {
		struct inet6_ifaddr *ifp;
		struct flowi6 fl_gw6;
		int redirect;

		ifp = ipv6_get_ifaddr(net, &fl6->saddr,
				      (*dst)->dev, 1);

		redirect = (ifp && ifp->flags & IFA_F_OPTIMISTIC);
		if (ifp)
			in6_ifa_put(ifp);

		if (redirect) {
			 
			dst_release(*dst);
			memcpy(&fl_gw6, fl6, sizeof(struct flowi6));
			memset(&fl_gw6.daddr, 0, sizeof(struct in6_addr));
			*dst = ip6_route_output(net, sk, &fl_gw6);
			if ((err = (*dst)->error))
				goto out_err_release;
		}
	}
#endif
	if (ipv6_addr_v4mapped(&fl6->saddr) &&
	    !(ipv6_addr_v4mapped(&fl6->daddr) || ipv6_addr_any(&fl6->daddr))) {
		err = -EAFNOSUPPORT;
		goto out_err_release;
	}

	return 0;

out_err_release:
	if (err == -ENETUNREACH)
		IP6_INC_STATS(net, NULL, IPSTATS_MIB_OUTNOROUTES);
	dst_release(*dst);
	*dst = NULL;
	return err;
}