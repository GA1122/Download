megasas_init_adapter_mfi(struct megasas_instance *instance)
{
	u32 context_sz;
	u32 reply_q_sz;

	 
	instance->max_fw_cmds = instance->instancet->read_fw_status_reg(instance) & 0x00FFFF;
	 
	instance->max_fw_cmds = instance->max_fw_cmds-1;
	instance->max_mfi_cmds = instance->max_fw_cmds;
	instance->max_num_sge = (instance->instancet->read_fw_status_reg(instance) & 0xFF0000) >>
					0x10;
	 
	if ((instance->pdev->device == PCI_DEVICE_ID_LSI_SAS0073SKINNY) ||
		(instance->pdev->device == PCI_DEVICE_ID_LSI_SAS0071SKINNY)) {
		instance->max_scsi_cmds = (instance->max_fw_cmds -
			MEGASAS_SKINNY_INT_CMDS);
		sema_init(&instance->ioctl_sem, MEGASAS_SKINNY_INT_CMDS);
	} else {
		instance->max_scsi_cmds = (instance->max_fw_cmds -
			MEGASAS_INT_CMDS);
		sema_init(&instance->ioctl_sem, (MEGASAS_MFI_IOCTL_CMDS));
	}

	instance->cur_can_queue = instance->max_scsi_cmds;
	 
	if (megasas_alloc_cmds(instance))
		goto fail_alloc_cmds;

	 
	context_sz = sizeof(u32);
	reply_q_sz = context_sz * (instance->max_fw_cmds + 1);

	instance->reply_queue = dma_alloc_coherent(&instance->pdev->dev,
			reply_q_sz, &instance->reply_queue_h, GFP_KERNEL);

	if (!instance->reply_queue) {
		dev_printk(KERN_DEBUG, &instance->pdev->dev, "Out of DMA mem for reply queue\n");
		goto fail_reply_queue;
	}

	if (megasas_issue_init_mfi(instance))
		goto fail_fw_init;

	if (megasas_get_ctrl_info(instance)) {
		dev_err(&instance->pdev->dev, "(%d): Could get controller info "
			"Fail from %s %d\n", instance->unique_id,
			__func__, __LINE__);
		goto fail_fw_init;
	}

	instance->fw_support_ieee = 0;
	instance->fw_support_ieee =
		(instance->instancet->read_fw_status_reg(instance) &
		0x04000000);

	dev_notice(&instance->pdev->dev, "megasas_init_mfi: fw_support_ieee=%d",
			instance->fw_support_ieee);

	if (instance->fw_support_ieee)
		instance->flag_ieee = 1;

	return 0;

fail_fw_init:

	dma_free_coherent(&instance->pdev->dev, reply_q_sz,
			    instance->reply_queue, instance->reply_queue_h);
fail_reply_queue:
	megasas_free_cmds(instance);

fail_alloc_cmds:
	return 1;
}