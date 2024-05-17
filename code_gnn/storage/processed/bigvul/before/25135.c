static void ipv4_dst_destroy(struct dst_entry *dst)
{
	struct rtable *rt = (struct rtable *) dst;
	struct inet_peer *peer = rt->peer;

	if (rt->fi) {
		fib_info_put(rt->fi);
		rt->fi = NULL;
	}
	if (peer) {
		rt->peer = NULL;
		inet_putpeer(peer);
	}
}
