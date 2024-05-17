int orderly_poweroff(bool force)
{
	int ret = __orderly_poweroff();

	if (ret && force) {
		printk(KERN_WARNING "Failed to start orderly shutdown: "
		       "forcing the issue\n");

		 
		emergency_sync();
		kernel_power_off();
	}

	return ret;
}
