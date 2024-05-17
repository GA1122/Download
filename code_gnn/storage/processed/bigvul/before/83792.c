static void hwsim_exit_netlink(void)
{
	 
	netlink_unregister_notifier(&hwsim_netlink_notifier);
	 
	genl_unregister_family(&hwsim_genl_family);
}
