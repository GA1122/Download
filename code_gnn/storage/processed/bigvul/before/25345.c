static int __init swp_emulation_init(void)
{
#ifdef CONFIG_PROC_FS
	struct proc_dir_entry *res;

	res = create_proc_entry("cpu/swp_emulation", S_IRUGO, NULL);

	if (!res)
		return -ENOMEM;

	res->read_proc = proc_read_status;
#endif  

	printk(KERN_NOTICE "Registering SWP/SWPB emulation handler\n");
	register_undef_hook(&swp_hook);

	return 0;
}