static void __exit exit_nlm(void)
{
	 
	nlm_shutdown_hosts();
	lockd_remove_procfs();
	unregister_pernet_subsys(&lockd_net_ops);
#ifdef CONFIG_SYSCTL
	unregister_sysctl_table(nlm_sysctl_table);
#endif
}
