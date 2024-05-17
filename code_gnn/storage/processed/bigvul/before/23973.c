static int __init airo_init_module( void )
{
	int i;

	airo_entry = proc_mkdir_mode("driver/aironet", airo_perm, NULL);

	if (airo_entry) {
		airo_entry->uid = proc_uid;
		airo_entry->gid = proc_gid;
	}

	for (i = 0; i < 4 && io[i] && irq[i]; i++) {
		airo_print_info("", "Trying to configure ISA adapter at irq=%d "
			"io=0x%x", irq[i], io[i] );
		if (init_airo_card( irq[i], io[i], 0, NULL ))
			  ;
	}

#ifdef CONFIG_PCI
	airo_print_info("", "Probing for PCI adapters");
	i = pci_register_driver(&airo_driver);
	airo_print_info("", "Finished probing for PCI adapters");

	if (i) {
		remove_proc_entry("driver/aironet", NULL);
		return i;
	}
#endif

	 
	return 0;
}