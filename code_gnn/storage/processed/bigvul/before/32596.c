static irqreturn_t tg3_interrupt(int irq, void *dev_id)
{
	struct tg3_napi *tnapi = dev_id;
	struct tg3 *tp = tnapi->tp;
	struct tg3_hw_status *sblk = tnapi->hw_status;
	unsigned int handled = 1;

	 
	if (unlikely(!(sblk->status & SD_STATUS_UPDATED))) {
		if (tg3_flag(tp, CHIP_RESETTING) ||
		    (tr32(TG3PCI_PCISTATE) & PCISTATE_INT_NOT_ACTIVE)) {
			handled = 0;
			goto out;
		}
	}

	 
	tw32_mailbox_f(MAILBOX_INTERRUPT_0 + TG3_64BIT_REG_LOW, 0x00000001);
	if (tg3_irq_sync(tp))
		goto out;
	sblk->status &= ~SD_STATUS_UPDATED;
	if (likely(tg3_has_work(tnapi))) {
		prefetch(&tnapi->rx_rcb[tnapi->rx_rcb_ptr]);
		napi_schedule(&tnapi->napi);
	} else {
		 
		tw32_mailbox_f(MAILBOX_INTERRUPT_0 + TG3_64BIT_REG_LOW,
			       0x00000000);
	}
out:
	return IRQ_RETVAL(handled);
}
