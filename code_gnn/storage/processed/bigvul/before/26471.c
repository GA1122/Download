static irqreturn_t pmcraid_isr_msix(int irq, void *dev_id)
{
	struct pmcraid_isr_param *hrrq_vector;
	struct pmcraid_instance *pinstance;
	unsigned long lock_flags;
	u32 intrs_val;
	int hrrq_id;

	hrrq_vector = (struct pmcraid_isr_param *)dev_id;
	hrrq_id = hrrq_vector->hrrq_id;
	pinstance = hrrq_vector->drv_inst;

	if (!hrrq_id) {
		 
		intrs_val = pmcraid_read_interrupts(pinstance);
		if (intrs_val &&
			((ioread32(pinstance->int_regs.host_ioa_interrupt_reg)
			& DOORBELL_INTR_MSIX_CLR) == 0)) {
			 
			if (intrs_val & PMCRAID_ERROR_INTERRUPTS) {
				if (intrs_val & INTRS_IOA_UNIT_CHECK)
					pinstance->ioa_unit_check = 1;

				pmcraid_err("ISR: error interrupts: %x \
					initiating reset\n", intrs_val);
				spin_lock_irqsave(pinstance->host->host_lock,
					lock_flags);
				pmcraid_initiate_reset(pinstance);
				spin_unlock_irqrestore(
					pinstance->host->host_lock,
					lock_flags);
			}
			 
			if (intrs_val & INTRS_TRANSITION_TO_OPERATIONAL)
				pmcraid_clr_trans_op(pinstance);

			 
			iowrite32(DOORBELL_INTR_MSIX_CLR,
				pinstance->int_regs.host_ioa_interrupt_reg);
			ioread32(pinstance->int_regs.host_ioa_interrupt_reg);


		}
	}

	tasklet_schedule(&(pinstance->isr_tasklet[hrrq_id]));

	return IRQ_HANDLED;
}
