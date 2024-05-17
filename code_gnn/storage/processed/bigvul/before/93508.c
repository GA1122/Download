static int __ip6mr_fill_mroute(struct mr6_table *mrt, struct sk_buff *skb,
			       struct mfc6_cache *c, struct rtmsg *rtm)
{
	struct rta_mfc_stats mfcs;
	struct nlattr *mp_attr;
	struct rtnexthop *nhp;
	unsigned long lastuse;
	int ct;

	 
	if (c->mf6c_parent >= MAXMIFS) {
		rtm->rtm_flags |= RTNH_F_UNRESOLVED;
		return -ENOENT;
	}

	if (MIF_EXISTS(mrt, c->mf6c_parent) &&
	    nla_put_u32(skb, RTA_IIF, mrt->vif6_table[c->mf6c_parent].dev->ifindex) < 0)
		return -EMSGSIZE;
	mp_attr = nla_nest_start(skb, RTA_MULTIPATH);
	if (!mp_attr)
		return -EMSGSIZE;

	for (ct = c->mfc_un.res.minvif; ct < c->mfc_un.res.maxvif; ct++) {
		if (MIF_EXISTS(mrt, ct) && c->mfc_un.res.ttls[ct] < 255) {
			nhp = nla_reserve_nohdr(skb, sizeof(*nhp));
			if (!nhp) {
				nla_nest_cancel(skb, mp_attr);
				return -EMSGSIZE;
			}

			nhp->rtnh_flags = 0;
			nhp->rtnh_hops = c->mfc_un.res.ttls[ct];
			nhp->rtnh_ifindex = mrt->vif6_table[ct].dev->ifindex;
			nhp->rtnh_len = sizeof(*nhp);
		}
	}

	nla_nest_end(skb, mp_attr);

	lastuse = READ_ONCE(c->mfc_un.res.lastuse);
	lastuse = time_after_eq(jiffies, lastuse) ? jiffies - lastuse : 0;

	mfcs.mfcs_packets = c->mfc_un.res.pkt;
	mfcs.mfcs_bytes = c->mfc_un.res.bytes;
	mfcs.mfcs_wrong_if = c->mfc_un.res.wrong_if;
	if (nla_put_64bit(skb, RTA_MFC_STATS, sizeof(mfcs), &mfcs, RTA_PAD) ||
	    nla_put_u64_64bit(skb, RTA_EXPIRES, jiffies_to_clock_t(lastuse),
			      RTA_PAD))
		return -EMSGSIZE;

	rtm->rtm_type = RTN_MULTICAST;
	return 1;
}