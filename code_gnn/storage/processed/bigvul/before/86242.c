static int __init i8042_toggle_aux(bool on)
{
	unsigned char param;
	int i;

	if (i8042_command(&param,
			on ? I8042_CMD_AUX_ENABLE : I8042_CMD_AUX_DISABLE))
		return -1;

	 
	for (i = 0; i < 100; i++) {
		udelay(50);

		if (i8042_command(&param, I8042_CMD_CTL_RCTR))
			return -1;

		if (!(param & I8042_CTR_AUXDIS) == on)
			return 0;
	}

	return -1;
}
