static int lockd_init_net(struct net *net)
{
	struct lockd_net *ln = net_generic(net, lockd_net_id);

	INIT_DELAYED_WORK(&ln->grace_period_end, grace_ender);
	INIT_LIST_HEAD(&ln->lockd_manager.list);
	ln->lockd_manager.block_opens = false;
	INIT_LIST_HEAD(&ln->nsm_handles);
	return 0;
}
