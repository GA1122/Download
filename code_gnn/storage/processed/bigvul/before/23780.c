static inline void slave_disable_netpoll(struct slave *slave)
{
	struct netpoll *np = slave->np;

	if (!np)
		return;

	slave->np = NULL;
	synchronize_rcu_bh();
	__netpoll_cleanup(np);
	kfree(np);
}
