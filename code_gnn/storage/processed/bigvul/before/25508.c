void bust_spinlocks(int yes)
{
	if (yes) {
		oops_in_progress = 1;
	} else {
		int loglevel_save = console_loglevel;
		console_unblank();
		oops_in_progress = 0;
		 
		console_loglevel = 15;
		printk(" ");
		console_loglevel = loglevel_save;
	}
}
