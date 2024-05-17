static int i8042_controller_init(void)
{
	unsigned long flags;
	int n = 0;
	unsigned char ctr[2];

 

	do {
		if (n >= 10) {
			pr_err("Unable to get stable CTR read\n");
			return -EIO;
		}

		if (n != 0)
			udelay(50);

		if (i8042_command(&ctr[n++ % 2], I8042_CMD_CTL_RCTR)) {
			pr_err("Can't read CTR while initializing i8042\n");
			return -EIO;
		}

	} while (n < 2 || ctr[0] != ctr[1]);

	i8042_initial_ctr = i8042_ctr = ctr[0];

 

	i8042_ctr |= I8042_CTR_KBDDIS;
	i8042_ctr &= ~I8042_CTR_KBDINT;

 

	spin_lock_irqsave(&i8042_lock, flags);
	if (~i8042_read_status() & I8042_STR_KEYLOCK) {
		if (i8042_unlock)
			i8042_ctr |= I8042_CTR_IGNKEYLOCK;
		else
			pr_warn("Warning: Keylock active\n");
	}
	spin_unlock_irqrestore(&i8042_lock, flags);

 

	if (~i8042_ctr & I8042_CTR_XLATE)
		i8042_direct = true;

 

	if (i8042_direct)
		i8042_ctr &= ~I8042_CTR_XLATE;

 

	if (i8042_command(&i8042_ctr, I8042_CMD_CTL_WCTR)) {
		pr_err("Can't write CTR while initializing i8042\n");
		return -EIO;
	}

 

	i8042_flush();

	return 0;
}
