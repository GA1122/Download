static irqreturn_t airo_interrupt(int irq, void *dev_id)
{
	struct net_device *dev = dev_id;
	u16 status, savedInterrupts = 0;
	struct airo_info *ai = dev->ml_priv;
	int handled = 0;

	if (!netif_device_present(dev))
		return IRQ_NONE;

	for (;;) {
		status = IN4500(ai, EVSTAT);
		if (!(status & STATUS_INTS) || (status == 0xffff))
			break;

		handled = 1;

		if (status & EV_AWAKE) {
			OUT4500(ai, EVACK, EV_AWAKE);
			OUT4500(ai, EVACK, EV_AWAKE);
		}

		if (!savedInterrupts) {
			savedInterrupts = IN4500(ai, EVINTEN);
			OUT4500(ai, EVINTEN, 0);
		}

		if (status & EV_MIC) {
			OUT4500(ai, EVACK, EV_MIC);
			airo_handle_cisco_mic(ai);
		}

		if (status & EV_LINK) {
			 
			airo_handle_link(ai);
		}

		 
		if (status & EV_RX)
			airo_handle_rx(ai);

		 
		if (status & (EV_TX | EV_TXCPY | EV_TXEXC))
			airo_handle_tx(ai, status);

		if ( status & ~STATUS_INTS & ~IGNORE_INTS ) {
			airo_print_warn(ai->dev->name, "Got weird status %x",
				status & ~STATUS_INTS & ~IGNORE_INTS );
		}
	}

	if (savedInterrupts)
		OUT4500(ai, EVINTEN, savedInterrupts);

	return IRQ_RETVAL(handled);
}