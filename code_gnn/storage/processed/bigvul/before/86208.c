static void i8042_controller_reset(bool s2r_wants_reset)
{
	i8042_flush();

 

	i8042_ctr |= I8042_CTR_KBDDIS | I8042_CTR_AUXDIS;
	i8042_ctr &= ~(I8042_CTR_KBDINT | I8042_CTR_AUXINT);

	if (i8042_command(&i8042_ctr, I8042_CMD_CTL_WCTR))
		pr_warn("Can't write CTR while resetting\n");

 

	if (i8042_mux_present)
		i8042_set_mux_mode(false, NULL);

 

	if (i8042_reset == I8042_RESET_ALWAYS ||
	    (i8042_reset == I8042_RESET_ON_S2RAM && s2r_wants_reset)) {
		i8042_controller_selftest();
	}

 

	if (i8042_command(&i8042_initial_ctr, I8042_CMD_CTL_WCTR))
		pr_warn("Can't restore CTR\n");
}