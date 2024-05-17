static int __init pfault_irq_init(void)
{
	int rc;

	if (!MACHINE_IS_VM)
		return 0;
	rc = register_external_interrupt(0x2603, pfault_interrupt);
	if (rc)
		goto out_extint;
	rc = pfault_init() == 0 ? 0 : -EOPNOTSUPP;
	if (rc)
		goto out_pfault;
	service_subclass_irq_register();
	hotcpu_notifier(pfault_cpu_notify, 0);
	return 0;

out_pfault:
	unregister_external_interrupt(0x2603, pfault_interrupt);
out_extint:
	pfault_disable = 1;
	return rc;
}
