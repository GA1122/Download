static int i8042_pm_suspend(struct device *dev)
{
	int i;

	if (pm_suspend_via_firmware())
		i8042_controller_reset(true);

	 
	for (i = 0; i < I8042_NUM_PORTS; i++) {
		struct serio *serio = i8042_ports[i].serio;

		if (serio && device_may_wakeup(&serio->dev))
			enable_irq_wake(i8042_ports[i].irq);
	}

	return 0;
}