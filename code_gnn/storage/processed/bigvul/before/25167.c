static inline void rt_drop(struct rtable *rt)
{
	ip_rt_put(rt);
	call_rcu_bh(&rt->dst.rcu_head, dst_rcu_free);
}
