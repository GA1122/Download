static int __init i8042_probe(struct platform_device *dev)
{
	int error;

	i8042_platform_device = dev;

	if (i8042_reset == I8042_RESET_ALWAYS) {
		error = i8042_controller_selftest();
		if (error)
			return error;
	}

	error = i8042_controller_init();
	if (error)
		return error;

#ifdef CONFIG_X86
	if (i8042_dritek)
		i8042_dritek_enable();
#endif

	if (!i8042_noaux) {
		error = i8042_setup_aux();
		if (error && error != -ENODEV && error != -EBUSY)
			goto out_fail;
	}

	if (!i8042_nokbd) {
		error = i8042_setup_kbd();
		if (error)
			goto out_fail;
	}
 
	i8042_register_ports();

	return 0;

 out_fail:
	i8042_free_aux_ports();	 
	i8042_free_irqs();
	i8042_controller_reset(false);
	i8042_platform_device = NULL;

	return error;
}