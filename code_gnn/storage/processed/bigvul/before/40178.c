static inline struct sock *udp_v4_mcast_next(struct net *net, struct sock *sk,
					     __be16 loc_port, __be32 loc_addr,
					     __be16 rmt_port, __be32 rmt_addr,
					     int dif)
{
	struct hlist_nulls_node *node;
	struct sock *s = sk;
	unsigned short hnum = ntohs(loc_port);

	sk_nulls_for_each_from(s, node) {
		if (__udp_is_mcast_sock(net, s,
					loc_port, loc_addr,
					rmt_port, rmt_addr,
					dif, hnum))
			goto found;
	}
	s = NULL;
found:
	return s;
}
