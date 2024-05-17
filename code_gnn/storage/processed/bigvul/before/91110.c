static void __ip_rt_update_pmtu(struct rtable *rt, struct flowi4 *fl4, u32 mtu)
{
	struct dst_entry *dst = &rt->dst;
	u32 old_mtu = ipv4_mtu(dst);
	struct fib_result res;
	bool lock = false;

	if (ip_mtu_locked(dst))
		return;

	if (old_mtu < mtu)
		return;

	if (mtu < ip_rt_min_pmtu) {
		lock = true;
		mtu = min(old_mtu, ip_rt_min_pmtu);
	}

	if (rt->rt_pmtu == mtu && !lock &&
	    time_before(jiffies, dst->expires - ip_rt_mtu_expires / 2))
		return;

	rcu_read_lock();
	if (fib_lookup(dev_net(dst->dev), fl4, &res, 0) == 0) {
		struct fib_nh *nh = &FIB_RES_NH(res);

		update_or_create_fnhe(nh, fl4->daddr, 0, mtu, lock,
				      jiffies + ip_rt_mtu_expires);
	}
	rcu_read_unlock();
}
