static void tg3_enable_register_access(struct tg3 *tp)
{
	 
	pci_write_config_dword(tp->pdev,
			       TG3PCI_MISC_HOST_CTRL, tp->misc_host_ctrl);
}