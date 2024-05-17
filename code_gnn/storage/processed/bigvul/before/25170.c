static inline int rt_fast_clean(struct rtable *rth)
{
	 
	return (rth->rt_flags & (RTCF_BROADCAST | RTCF_MULTICAST)) &&
		rt_is_input_route(rth) && rth->dst.rt_next;
}
