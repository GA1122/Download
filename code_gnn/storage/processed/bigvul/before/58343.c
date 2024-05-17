void machine_restart(char *cmd)
{
	machine_shutdown();

	arm_pm_restart(reboot_mode, cmd);

	 
	mdelay(1000);

	 
	printk("Reboot failed -- System halted\n");
	local_irq_disable();
	while (1);
}
