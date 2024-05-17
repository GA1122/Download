void rt_cache_flush_batch(struct net *net)
{
	rt_do_flush(net, !in_softirq());
}
