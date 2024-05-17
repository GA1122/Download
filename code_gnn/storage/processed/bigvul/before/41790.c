void addrconf_prefix_rcv(struct net_device *dev, u8 *opt, int len, bool sllao)
{
	struct prefix_info *pinfo;
	__u32 valid_lft;
	__u32 prefered_lft;
	int addr_type;
	struct inet6_dev *in6_dev;
	struct net *net = dev_net(dev);

	pinfo = (struct prefix_info *) opt;

	if (len < sizeof(struct prefix_info)) {
		ADBG("addrconf: prefix option too short\n");
		return;
	}

	 

	addr_type = ipv6_addr_type(&pinfo->prefix);

	if (addr_type & (IPV6_ADDR_MULTICAST|IPV6_ADDR_LINKLOCAL))
		return;

	valid_lft = ntohl(pinfo->valid);
	prefered_lft = ntohl(pinfo->prefered);

	if (prefered_lft > valid_lft) {
		net_warn_ratelimited("addrconf: prefix option has invalid lifetime\n");
		return;
	}

	in6_dev = in6_dev_get(dev);

	if (in6_dev == NULL) {
		net_dbg_ratelimited("addrconf: device %s not configured\n",
				    dev->name);
		return;
	}

	 

	if (pinfo->onlink) {
		struct rt6_info *rt;
		unsigned long rt_expires;

		 
		if (HZ > USER_HZ)
			rt_expires = addrconf_timeout_fixup(valid_lft, HZ);
		else
			rt_expires = addrconf_timeout_fixup(valid_lft, USER_HZ);

		if (addrconf_finite_timeout(rt_expires))
			rt_expires *= HZ;

		rt = addrconf_get_prefix_route(&pinfo->prefix,
					       pinfo->prefix_len,
					       dev,
					       RTF_ADDRCONF | RTF_PREFIX_RT,
					       RTF_GATEWAY | RTF_DEFAULT);

		if (rt) {
			 
			if (valid_lft == 0) {
				ip6_del_rt(rt);
				rt = NULL;
			} else if (addrconf_finite_timeout(rt_expires)) {
				 
				rt6_set_expires(rt, jiffies + rt_expires);
			} else {
				rt6_clean_expires(rt);
			}
		} else if (valid_lft) {
			clock_t expires = 0;
			int flags = RTF_ADDRCONF | RTF_PREFIX_RT;
			if (addrconf_finite_timeout(rt_expires)) {
				 
				flags |= RTF_EXPIRES;
				expires = jiffies_to_clock_t(rt_expires);
			}
			addrconf_prefix_route(&pinfo->prefix, pinfo->prefix_len,
					      dev, expires, flags);
		}
		ip6_rt_put(rt);
	}

	 

	if (pinfo->autoconf && in6_dev->cnf.autoconf) {
		struct inet6_ifaddr *ifp;
		struct in6_addr addr;
		int create = 0, update_lft = 0;
		bool tokenized = false;

		if (pinfo->prefix_len == 64) {
			memcpy(&addr, &pinfo->prefix, 8);

			if (!ipv6_addr_any(&in6_dev->token)) {
				read_lock_bh(&in6_dev->lock);
				memcpy(addr.s6_addr + 8,
				       in6_dev->token.s6_addr + 8, 8);
				read_unlock_bh(&in6_dev->lock);
				tokenized = true;
			} else if (ipv6_generate_eui64(addr.s6_addr + 8, dev) &&
				   ipv6_inherit_eui64(addr.s6_addr + 8, in6_dev)) {
				in6_dev_put(in6_dev);
				return;
			}
			goto ok;
		}
		net_dbg_ratelimited("IPv6 addrconf: prefix with wrong length %d\n",
				    pinfo->prefix_len);
		in6_dev_put(in6_dev);
		return;

ok:

		ifp = ipv6_get_ifaddr(net, &addr, dev, 1);

		if (ifp == NULL && valid_lft) {
			int max_addresses = in6_dev->cnf.max_addresses;
			u32 addr_flags = 0;

#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
			if (in6_dev->cnf.optimistic_dad &&
			    !net->ipv6.devconf_all->forwarding && sllao)
				addr_flags = IFA_F_OPTIMISTIC;
#endif

			 
			if (!max_addresses ||
			    ipv6_count_addresses(in6_dev) < max_addresses)
				ifp = ipv6_add_addr(in6_dev, &addr, NULL,
						    pinfo->prefix_len,
						    addr_type&IPV6_ADDR_SCOPE_MASK,
						    addr_flags, valid_lft,
						    prefered_lft);

			if (IS_ERR_OR_NULL(ifp)) {
				in6_dev_put(in6_dev);
				return;
			}

			update_lft = 0;
			create = 1;
			spin_lock_bh(&ifp->lock);
			ifp->flags |= IFA_F_MANAGETEMPADDR;
			ifp->cstamp = jiffies;
			ifp->tokenized = tokenized;
			spin_unlock_bh(&ifp->lock);
			addrconf_dad_start(ifp);
		}

		if (ifp) {
			u32 flags;
			unsigned long now;
			u32 stored_lft;

			 
			spin_lock(&ifp->lock);
			now = jiffies;
			if (ifp->valid_lft > (now - ifp->tstamp) / HZ)
				stored_lft = ifp->valid_lft - (now - ifp->tstamp) / HZ;
			else
				stored_lft = 0;
			if (!update_lft && !create && stored_lft) {
				const u32 minimum_lft = min_t(u32,
					stored_lft, MIN_VALID_LIFETIME);
				valid_lft = max(valid_lft, minimum_lft);

				 
				update_lft = 1;
			}

			if (update_lft) {
				ifp->valid_lft = valid_lft;
				ifp->prefered_lft = prefered_lft;
				ifp->tstamp = now;
				flags = ifp->flags;
				ifp->flags &= ~IFA_F_DEPRECATED;
				spin_unlock(&ifp->lock);

				if (!(flags&IFA_F_TENTATIVE))
					ipv6_ifa_notify(0, ifp);
			} else
				spin_unlock(&ifp->lock);

			manage_tempaddrs(in6_dev, ifp, valid_lft, prefered_lft,
					 create, now);

			in6_ifa_put(ifp);
			addrconf_verify();
		}
	}
	inet6_prefix_notify(RTM_NEWPREFIX, in6_dev, pinfo);
	in6_dev_put(in6_dev);
}