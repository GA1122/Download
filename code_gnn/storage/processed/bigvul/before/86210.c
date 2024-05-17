static int i8042_controller_selftest(void)
{
	unsigned char param;
	int i = 0;

	 
	do {

		if (i8042_command(&param, I8042_CMD_CTL_TEST)) {
			pr_err("i8042 controller selftest timeout\n");
			return -ENODEV;
		}

		if (param == I8042_RET_CTL_TEST)
			return 0;

		dbg("i8042 controller selftest: %#x != %#x\n",
		    param, I8042_RET_CTL_TEST);
		msleep(50);
	} while (i++ < 5);

#ifdef CONFIG_X86
	 
	pr_info("giving up on controller selftest, continuing anyway...\n");
	return 0;
#else
	pr_err("i8042 controller selftest failed\n");
	return -EIO;
#endif
}
