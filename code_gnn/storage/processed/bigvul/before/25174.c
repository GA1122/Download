static inline int rt_genid(struct net *net)
{
	return atomic_read(&net->ipv4.rt_genid);
}
