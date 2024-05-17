static void restart_grace(void)
{
	if (nlmsvc_ops) {
		struct net *net = &init_net;
		struct lockd_net *ln = net_generic(net, lockd_net_id);

		cancel_delayed_work_sync(&ln->grace_period_end);
		locks_end_grace(&ln->lockd_manager);
		nlmsvc_invalidate_all();
		set_grace_period(net);
	}
}
