static void ext4_destroy_lazyinit_thread(void)
{
	 
	if (!ext4_li_info || !ext4_lazyinit_task)
		return;

	kthread_stop(ext4_lazyinit_task);
}
