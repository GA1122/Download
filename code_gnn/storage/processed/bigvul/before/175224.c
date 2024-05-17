void kernel_restart(char *cmd)
{
	kernel_restart_prepare(cmd);
	migrate_to_reboot_cpu();
	syscore_shutdown();
 if (!cmd)
		printk(KERN_EMERG "Restarting system.\n");
 else
		printk(KERN_EMERG "Restarting system with command '%s'.\n", cmd);
	kmsg_dump(KMSG_DUMP_RESTART);
	machine_restart(cmd);
}
