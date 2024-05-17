static void rt_emergency_hash_rebuild(struct net *net)
{
	if (net_ratelimit())
		printk(KERN_WARNING "Route hash chain too long!\n");
	rt_cache_invalidate(net);
}
