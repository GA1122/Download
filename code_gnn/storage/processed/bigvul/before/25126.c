static void ip_rt_update_pmtu(struct dst_entry *dst, u32 mtu)
{
	struct rtable *rt = (struct rtable *) dst;
	struct inet_peer *peer;

	dst_confirm(dst);

	if (!rt->peer)
		rt_bind_peer(rt, rt->rt_dst, 1);
	peer = rt->peer;
	if (peer) {
		unsigned long pmtu_expires = ACCESS_ONCE(peer->pmtu_expires);

		if (mtu < ip_rt_min_pmtu)
			mtu = ip_rt_min_pmtu;
		if (!pmtu_expires || mtu < peer->pmtu_learned) {

			pmtu_expires = jiffies + ip_rt_mtu_expires;
			if (!pmtu_expires)
				pmtu_expires = 1UL;

			peer->pmtu_learned = mtu;
			peer->pmtu_expires = pmtu_expires;

			atomic_inc(&__rt_peer_genid);
			rt->rt_peer_genid = rt_peer_genid();
		}
		check_peer_pmtu(dst, peer);
	}
}
