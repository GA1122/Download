void megaraid_sas_kill_hba(struct megasas_instance *instance)
{
	 
	atomic_set(&instance->adprecovery, MEGASAS_HW_CRITICAL_ERROR);
	 
	msleep(1000);
	if ((instance->pdev->device == PCI_DEVICE_ID_LSI_SAS0073SKINNY) ||
		(instance->pdev->device == PCI_DEVICE_ID_LSI_SAS0071SKINNY) ||
		(instance->adapter_type != MFI_SERIES)) {
		if (!instance->requestorId) {
			writel(MFI_STOP_ADP, &instance->reg_set->doorbell);
			 
			readl(&instance->reg_set->doorbell);
		}
		if (instance->requestorId && instance->peerIsPresent)
			memset(instance->ld_ids, 0xff, MEGASAS_MAX_LD_IDS);
	} else {
		writel(MFI_STOP_ADP,
			&instance->reg_set->inbound_doorbell);
	}
	 
	megasas_complete_outstanding_ioctls(instance);
}
