static int __init i8042_check_mux(void)
{
	unsigned char mux_version;

	if (i8042_set_mux_mode(true, &mux_version))
		return -1;

	pr_info("Detected active multiplexing controller, rev %d.%d\n",
		(mux_version >> 4) & 0xf, mux_version & 0xf);

 
	i8042_ctr |= I8042_CTR_AUXDIS;
	i8042_ctr &= ~I8042_CTR_AUXINT;

	if (i8042_command(&i8042_ctr, I8042_CMD_CTL_WCTR)) {
		pr_err("Failed to disable AUX port, can't use MUX\n");
		return -EIO;
	}

	i8042_mux_present = true;

	return 0;
}