static struct dst_entry *ipv4_dst_check(struct dst_entry *dst, u32 cookie)
{
	struct rtable *rt = (struct rtable *) dst;

	if (rt_is_expired(rt))
		return NULL;
	if (rt->rt_peer_genid != rt_peer_genid()) {
		struct inet_peer *peer;

		if (!rt->peer)
			rt_bind_peer(rt, rt->rt_dst, 0);

		peer = rt->peer;
		if (peer) {
			check_peer_pmtu(dst, peer);

			if (peer->redirect_learned.a4 &&
			    peer->redirect_learned.a4 != rt->rt_gateway) {
				if (check_peer_redir(dst, peer))
					return NULL;
			}
		}

		rt->rt_peer_genid = rt_peer_genid();
	}
	return dst;
}
