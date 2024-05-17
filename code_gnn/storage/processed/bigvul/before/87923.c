static void __exit fm10k_exit_module(void)
{
	fm10k_unregister_pci_driver();

	fm10k_dbg_exit();

	 
	destroy_workqueue(fm10k_workqueue);
}
