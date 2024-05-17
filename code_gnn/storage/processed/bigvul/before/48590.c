static int __init vfio_pci_init(void)
{
	int ret;

	 
	ret = vfio_pci_init_perm_bits();
	if (ret)
		return ret;

	 
	ret = pci_register_driver(&vfio_pci_driver);
	if (ret)
		goto out_driver;

	vfio_pci_fill_ids();

	return 0;

out_driver:
	vfio_pci_uninit_perm_bits();
	return ret;
}
