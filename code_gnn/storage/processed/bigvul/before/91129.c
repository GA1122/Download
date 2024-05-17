struct rtable *ip_route_output_key_hash(struct net *net, struct flowi4 *fl4,
					const struct sk_buff *skb)
{
	__u8 tos = RT_FL_TOS(fl4);
	struct fib_result res = {
		.type		= RTN_UNSPEC,
		.fi		= NULL,
		.table		= NULL,
		.tclassid	= 0,
	};
	struct rtable *rth;

	fl4->flowi4_iif = LOOPBACK_IFINDEX;
	fl4->flowi4_tos = tos & IPTOS_RT_MASK;
	fl4->flowi4_scope = ((tos & RTO_ONLINK) ?
			 RT_SCOPE_LINK : RT_SCOPE_UNIVERSE);

	rcu_read_lock();
	rth = ip_route_output_key_hash_rcu(net, fl4, &res, skb);
	rcu_read_unlock();

	return rth;
}
