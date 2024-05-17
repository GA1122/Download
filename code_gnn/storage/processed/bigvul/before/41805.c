static void addrconf_verify_rtnl(void)
{
	unsigned long now, next, next_sec, next_sched;
	struct inet6_ifaddr *ifp;
	int i;

	ASSERT_RTNL();

	rcu_read_lock_bh();
	now = jiffies;
	next = round_jiffies_up(now + ADDR_CHECK_FREQUENCY);

	cancel_delayed_work(&addr_chk_work);

	for (i = 0; i < IN6_ADDR_HSIZE; i++) {
restart:
		hlist_for_each_entry_rcu_bh(ifp, &inet6_addr_lst[i], addr_lst) {
			unsigned long age;

			 
			if ((ifp->flags & IFA_F_PERMANENT) &&
			    (ifp->prefered_lft == INFINITY_LIFE_TIME))
				continue;

			spin_lock(&ifp->lock);
			 
			age = (now - ifp->tstamp + ADDRCONF_TIMER_FUZZ_MINUS) / HZ;

			if (ifp->valid_lft != INFINITY_LIFE_TIME &&
			    age >= ifp->valid_lft) {
				spin_unlock(&ifp->lock);
				in6_ifa_hold(ifp);
				ipv6_del_addr(ifp);
				goto restart;
			} else if (ifp->prefered_lft == INFINITY_LIFE_TIME) {
				spin_unlock(&ifp->lock);
				continue;
			} else if (age >= ifp->prefered_lft) {
				 
				int deprecate = 0;

				if (!(ifp->flags&IFA_F_DEPRECATED)) {
					deprecate = 1;
					ifp->flags |= IFA_F_DEPRECATED;
				}

				if ((ifp->valid_lft != INFINITY_LIFE_TIME) &&
				    (time_before(ifp->tstamp + ifp->valid_lft * HZ, next)))
					next = ifp->tstamp + ifp->valid_lft * HZ;

				spin_unlock(&ifp->lock);

				if (deprecate) {
					in6_ifa_hold(ifp);

					ipv6_ifa_notify(0, ifp);
					in6_ifa_put(ifp);
					goto restart;
				}
			} else if ((ifp->flags&IFA_F_TEMPORARY) &&
				   !(ifp->flags&IFA_F_TENTATIVE)) {
				unsigned long regen_advance = ifp->idev->cnf.regen_max_retry *
					ifp->idev->cnf.dad_transmits *
					NEIGH_VAR(ifp->idev->nd_parms, RETRANS_TIME) / HZ;

				if (age >= ifp->prefered_lft - regen_advance) {
					struct inet6_ifaddr *ifpub = ifp->ifpub;
					if (time_before(ifp->tstamp + ifp->prefered_lft * HZ, next))
						next = ifp->tstamp + ifp->prefered_lft * HZ;
					if (!ifp->regen_count && ifpub) {
						ifp->regen_count++;
						in6_ifa_hold(ifp);
						in6_ifa_hold(ifpub);
						spin_unlock(&ifp->lock);

						spin_lock(&ifpub->lock);
						ifpub->regen_count = 0;
						spin_unlock(&ifpub->lock);
						ipv6_create_tempaddr(ifpub, ifp);
						in6_ifa_put(ifpub);
						in6_ifa_put(ifp);
						goto restart;
					}
				} else if (time_before(ifp->tstamp + ifp->prefered_lft * HZ - regen_advance * HZ, next))
					next = ifp->tstamp + ifp->prefered_lft * HZ - regen_advance * HZ;
				spin_unlock(&ifp->lock);
			} else {
				 
				if (time_before(ifp->tstamp + ifp->prefered_lft * HZ, next))
					next = ifp->tstamp + ifp->prefered_lft * HZ;
				spin_unlock(&ifp->lock);
			}
		}
	}

	next_sec = round_jiffies_up(next);
	next_sched = next;

	 
	if (time_before(next_sec, next + ADDRCONF_TIMER_FUZZ))
		next_sched = next_sec;

	 
	if (time_before(next_sched, jiffies + ADDRCONF_TIMER_FUZZ_MAX))
		next_sched = jiffies + ADDRCONF_TIMER_FUZZ_MAX;

	ADBG(KERN_DEBUG "now = %lu, schedule = %lu, rounded schedule = %lu => %lu\n",
	      now, next, next_sec, next_sched);
	mod_delayed_work(addrconf_wq, &addr_chk_work, next_sched - now);
	rcu_read_unlock_bh();
}