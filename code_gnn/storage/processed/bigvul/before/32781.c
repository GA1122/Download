static int tg3_test_msi(struct tg3 *tp)
{
	int err;
	u16 pci_cmd;

	if (!tg3_flag(tp, USING_MSI))
		return 0;

	 
	pci_read_config_word(tp->pdev, PCI_COMMAND, &pci_cmd);
	pci_write_config_word(tp->pdev, PCI_COMMAND,
			      pci_cmd & ~PCI_COMMAND_SERR);

	err = tg3_test_interrupt(tp);

	pci_write_config_word(tp->pdev, PCI_COMMAND, pci_cmd);

	if (!err)
		return 0;

	 
	if (err != -EIO)
		return err;

	 
	netdev_warn(tp->dev, "No interrupt was generated using MSI. Switching "
		    "to INTx mode. Please report this failure to the PCI "
		    "maintainer and include system chipset information\n");

	free_irq(tp->napi[0].irq_vec, &tp->napi[0]);

	pci_disable_msi(tp->pdev);

	tg3_flag_clear(tp, USING_MSI);
	tp->napi[0].irq_vec = tp->pdev->irq;

	err = tg3_request_irq(tp, 0);
	if (err)
		return err;

	 
	tg3_full_lock(tp, 1);

	tg3_halt(tp, RESET_KIND_SHUTDOWN, 1);
	err = tg3_init_hw(tp, 1);

	tg3_full_unlock(tp);

	if (err)
		free_irq(tp->napi[0].irq_vec, &tp->napi[0]);

	return err;
}
