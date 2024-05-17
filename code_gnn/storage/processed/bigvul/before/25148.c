void rt_bind_peer(struct rtable *rt, __be32 daddr, int create)
{
	struct inet_peer *peer;

	peer = inet_getpeer_v4(daddr, create);

	if (peer && cmpxchg(&rt->peer, NULL, peer) != NULL)
		inet_putpeer(peer);
	else
		rt->rt_peer_genid = rt_peer_genid();
}
