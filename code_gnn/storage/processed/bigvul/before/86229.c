static int i8042_pm_resume(struct device *dev)
{
	bool want_reset;
	int i;

	for (i = 0; i < I8042_NUM_PORTS; i++) {
		struct serio *serio = i8042_ports[i].serio;

		if (serio && device_may_wakeup(&serio->dev))
			disable_irq_wake(i8042_ports[i].irq);
	}

	 
	if (!pm_suspend_via_firmware())
		return 0;

	 
	want_reset = pm_resume_via_firmware();

	return i8042_controller_resume(want_reset);
}
