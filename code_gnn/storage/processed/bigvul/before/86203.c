static int __init i8042_check_aux(void)
{
	int retval = -1;
	bool irq_registered = false;
	bool aux_loop_broken = false;
	unsigned long flags;
	unsigned char param;

 

	i8042_flush();

 

	param = 0x5a;
	retval = i8042_command(&param, I8042_CMD_AUX_LOOP);
	if (retval || param != 0x5a) {

 

		if (i8042_command(&param, I8042_CMD_AUX_TEST) ||
		    (param && param != 0xfa && param != 0xff))
			return -1;

 
		if (!retval)
			aux_loop_broken = true;
	}

 

	if (i8042_toggle_aux(false)) {
		pr_warn("Failed to disable AUX port, but continuing anyway... Is this a SiS?\n");
		pr_warn("If AUX port is really absent please use the 'i8042.noaux' option\n");
	}

	if (i8042_toggle_aux(true))
		return -1;

 
	if (i8042_kbdreset) {
		pr_warn("Attempting to reset device connected to KBD port\n");
		i8042_kbd_write(NULL, (unsigned char) 0xff);
	}

 

	if (i8042_noloop || i8042_bypass_aux_irq_test || aux_loop_broken) {
 
		retval = 0;
		goto out;
	}

	if (request_irq(I8042_AUX_IRQ, i8042_aux_test_irq, IRQF_SHARED,
			"i8042", i8042_platform_device))
		goto out;

	irq_registered = true;

	if (i8042_enable_aux_port())
		goto out;

	spin_lock_irqsave(&i8042_lock, flags);

	init_completion(&i8042_aux_irq_delivered);
	i8042_irq_being_tested = true;

	param = 0xa5;
	retval = __i8042_command(&param, I8042_CMD_AUX_LOOP & 0xf0ff);

	spin_unlock_irqrestore(&i8042_lock, flags);

	if (retval)
		goto out;

	if (wait_for_completion_timeout(&i8042_aux_irq_delivered,
					msecs_to_jiffies(250)) == 0) {
 
		dbg("     -- i8042 (aux irq test timeout)\n");
		i8042_flush();
		retval = -1;
	}

 out:

 

	i8042_ctr |= I8042_CTR_AUXDIS;
	i8042_ctr &= ~I8042_CTR_AUXINT;

	if (i8042_command(&i8042_ctr, I8042_CMD_CTL_WCTR))
		retval = -1;

	if (irq_registered)
		free_irq(I8042_AUX_IRQ, i8042_platform_device);

	return retval;
}