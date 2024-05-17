static void shutdown_smi(void *send_info)
{
	struct smi_info *smi_info = send_info;

	if (smi_info->dev_group_added) {
		device_remove_group(smi_info->io.dev, &ipmi_si_dev_attr_group);
		smi_info->dev_group_added = false;
	}
	if (smi_info->io.dev)
		dev_set_drvdata(smi_info->io.dev, NULL);

	 
	smi_info->interrupt_disabled = true;
	if (smi_info->io.irq_cleanup) {
		smi_info->io.irq_cleanup(&smi_info->io);
		smi_info->io.irq_cleanup = NULL;
	}
	stop_timer_and_thread(smi_info);

	 
	synchronize_rcu();

	 
	while (smi_info->curr_msg || (smi_info->si_state != SI_NORMAL)) {
		poll(smi_info);
		schedule_timeout_uninterruptible(1);
	}
	if (smi_info->handlers)
		disable_si_irq(smi_info);
	while (smi_info->curr_msg || (smi_info->si_state != SI_NORMAL)) {
		poll(smi_info);
		schedule_timeout_uninterruptible(1);
	}
	if (smi_info->handlers)
		smi_info->handlers->cleanup(smi_info->si_sm);

	if (smi_info->io.addr_source_cleanup) {
		smi_info->io.addr_source_cleanup(&smi_info->io);
		smi_info->io.addr_source_cleanup = NULL;
	}
	if (smi_info->io.io_cleanup) {
		smi_info->io.io_cleanup(&smi_info->io);
		smi_info->io.io_cleanup = NULL;
	}

	kfree(smi_info->si_sm);
	smi_info->si_sm = NULL;

	smi_info->intf = NULL;
}
