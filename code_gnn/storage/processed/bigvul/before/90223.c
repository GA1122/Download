static int init_ipmi_si(void)
{
	struct smi_info *e;
	enum ipmi_addr_src type = SI_INVALID;

	if (initialized)
		return 0;

	pr_info("IPMI System Interface driver\n");

	 
	if (!ipmi_si_hardcode_find_bmc())
		goto do_scan;

	ipmi_si_platform_init();

	ipmi_si_pci_init();

	ipmi_si_parisc_init();

	 
do_scan:
	mutex_lock(&smi_infos_lock);
	list_for_each_entry(e, &smi_infos, link) {
		 
		if (e->io.irq && (!type || e->io.addr_source == type)) {
			if (!try_smi_init(e)) {
				type = e->io.addr_source;
			}
		}
	}

	 
	if (type)
		goto skip_fallback_noirq;

	 

	list_for_each_entry(e, &smi_infos, link) {
		if (!e->io.irq && (!type || e->io.addr_source == type)) {
			if (!try_smi_init(e)) {
				type = e->io.addr_source;
			}
		}
	}

skip_fallback_noirq:
	initialized = 1;
	mutex_unlock(&smi_infos_lock);

	if (type)
		return 0;

	mutex_lock(&smi_infos_lock);
	if (unload_when_empty && list_empty(&smi_infos)) {
		mutex_unlock(&smi_infos_lock);
		cleanup_ipmi_si();
		pr_warn("Unable to find any System Interface(s)\n");
		return -ENODEV;
	} else {
		mutex_unlock(&smi_infos_lock);
		return 0;
	}
}
