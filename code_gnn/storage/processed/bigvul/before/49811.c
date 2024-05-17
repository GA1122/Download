static void arcmsr_iop_init(struct AdapterControlBlock *acb)
{
	uint32_t intmask_org;
	 
	intmask_org = arcmsr_disable_outbound_ints(acb);
	arcmsr_wait_firmware_ready(acb);
	arcmsr_iop_confirm(acb);
	 
	arcmsr_start_adapter_bgrb(acb);
	 
	arcmsr_clear_doorbell_queue_buffer(acb);
	arcmsr_enable_eoi_mode(acb);
	 
	arcmsr_enable_outbound_ints(acb, intmask_org);
	acb->acb_flags |= ACB_F_IOP_INITED;
}
