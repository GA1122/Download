static int __init keep_bootcon_setup(char *str)
{
	keep_bootcon = 1;
	printk(KERN_INFO "debug: skip boot console de-registration.\n");

	return 0;
}
