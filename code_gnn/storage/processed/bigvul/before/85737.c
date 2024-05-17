static void __exit exit_o2nm(void)
{
	 
	o2net_unregister_hb_callbacks();
	configfs_unregister_subsystem(&o2nm_cluster_group.cs_subsys);
	o2cb_sys_shutdown();

	o2net_exit();
	o2hb_exit();
}
