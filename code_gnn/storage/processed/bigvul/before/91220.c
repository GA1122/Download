static void __exit cleanup_ipmi(void)
{
	int count;

	if (!initialized)
		return;

	atomic_notifier_chain_unregister(&panic_notifier_list, &panic_block);

	 

	 
	atomic_inc(&stop_operation);
	del_timer_sync(&ipmi_timer);

	driver_unregister(&ipmidriver.driver);

	initialized = 0;

	 
	count = atomic_read(&smi_msg_inuse_count);
	if (count != 0)
		pr_warn("SMI message count %d at exit\n", count);
	count = atomic_read(&recv_msg_inuse_count);
	if (count != 0)
		pr_warn("recv message count %d at exit\n", count);
}
