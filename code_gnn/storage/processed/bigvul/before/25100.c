static int check_peer_redir(struct dst_entry *dst, struct inet_peer *peer)
{
	struct rtable *rt = (struct rtable *) dst;
	__be32 orig_gw = rt->rt_gateway;
	struct neighbour *n, *old_n;

	dst_confirm(&rt->dst);

	rt->rt_gateway = peer->redirect_learned.a4;

	n = ipv4_neigh_lookup(&rt->dst, &rt->rt_gateway);
	if (IS_ERR(n))
		return PTR_ERR(n);
	old_n = xchg(&rt->dst._neighbour, n);
	if (old_n)
		neigh_release(old_n);
	if (!n || !(n->nud_state & NUD_VALID)) {
		if (n)
			neigh_event_send(n, NULL);
		rt->rt_gateway = orig_gw;
		return -EAGAIN;
	} else {
		rt->rt_flags |= RTCF_REDIRECTED;
		call_netevent_notifiers(NETEVENT_NEIGH_UPDATE, n);
	}
	return 0;
}
