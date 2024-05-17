static int airo_pci_suspend(struct pci_dev *pdev, pm_message_t state)
{
	struct net_device *dev = pci_get_drvdata(pdev);
	struct airo_info *ai = dev->ml_priv;
	Cmd cmd;
	Resp rsp;

	if (!ai->APList)
		ai->APList = kmalloc(sizeof(APListRid), GFP_KERNEL);
	if (!ai->APList)
		return -ENOMEM;
	if (!ai->SSID)
		ai->SSID = kmalloc(sizeof(SsidRid), GFP_KERNEL);
	if (!ai->SSID)
		return -ENOMEM;
	readAPListRid(ai, ai->APList);
	readSsidRid(ai, ai->SSID);
	memset(&cmd, 0, sizeof(cmd));
	 
	if (down_interruptible(&ai->sem))
		return -EAGAIN;
	disable_MAC(ai, 0);
	netif_device_detach(dev);
	ai->power = state;
	cmd.cmd = HOSTSLEEP;
	issuecommand(ai, &cmd, &rsp);

	pci_enable_wake(pdev, pci_choose_state(pdev, state), 1);
	pci_save_state(pdev);
	pci_set_power_state(pdev, pci_choose_state(pdev, state));
	return 0;
}