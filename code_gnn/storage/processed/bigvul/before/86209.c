static int i8042_controller_resume(bool s2r_wants_reset)
{
	int error;

	error = i8042_controller_check();
	if (error)
		return error;

	if (i8042_reset == I8042_RESET_ALWAYS ||
	    (i8042_reset == I8042_RESET_ON_S2RAM && s2r_wants_reset)) {
		error = i8042_controller_selftest();
		if (error)
			return error;
	}

 

	i8042_ctr = i8042_initial_ctr;
	if (i8042_direct)
		i8042_ctr &= ~I8042_CTR_XLATE;
	i8042_ctr |= I8042_CTR_AUXDIS | I8042_CTR_KBDDIS;
	i8042_ctr &= ~(I8042_CTR_AUXINT | I8042_CTR_KBDINT);
	if (i8042_command(&i8042_ctr, I8042_CMD_CTL_WCTR)) {
		pr_warn("Can't write CTR to resume, retrying...\n");
		msleep(50);
		if (i8042_command(&i8042_ctr, I8042_CMD_CTL_WCTR)) {
			pr_err("CTR write retry failed\n");
			return -EIO;
		}
	}


#ifdef CONFIG_X86
	if (i8042_dritek)
		i8042_dritek_enable();
#endif

	if (i8042_mux_present) {
		if (i8042_set_mux_mode(true, NULL) || i8042_enable_mux_ports())
			pr_warn("failed to resume active multiplexor, mouse won't work\n");
	} else if (i8042_ports[I8042_AUX_PORT_NO].serio)
		i8042_enable_aux_port();

	if (i8042_ports[I8042_KBD_PORT_NO].serio)
		i8042_enable_kbd_port();

	i8042_interrupt(0, NULL);

	return 0;
}