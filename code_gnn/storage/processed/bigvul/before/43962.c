static int bar_write(struct pci_dev *dev, int offset, u32 value, void *data)
{
	struct pci_bar_info *bar = data;

	if (unlikely(!bar)) {
		pr_warn(DRV_NAME ": driver data not found for %s\n",
		       pci_name(dev));
		return XEN_PCI_ERR_op_failed;
	}

	 
	if (value == ~0)
		bar->which = 1;
	else {
		u32 tmpval;
		pci_read_config_dword(dev, offset, &tmpval);
		if (tmpval != bar->val && value == bar->val) {
			 
			pci_write_config_dword(dev, offset, bar->val);
		}
		bar->which = 0;
	}

	return 0;
}