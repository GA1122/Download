fst_intr(int dummy, void *dev_id)
{
	struct fst_card_info *card = dev_id;
	struct fst_port_info *port;
	int rdidx;		 
	int wridx;
	int event;		 
	unsigned int dma_intcsr = 0;
	unsigned int do_card_interrupt;
	unsigned int int_retry_count;

	 
	dbg(DBG_INTR, "intr: %d %p\n", card->irq, card);
	if (card->state != FST_RUNNING) {
		pr_err("Interrupt received for card %d in a non running state (%d)\n",
		       card->card_no, card->state);

		 
		fst_clear_intr(card);
		return IRQ_HANDLED;
	}

	 
	fst_clear_intr(card);

	 
	do_card_interrupt = 0;
	if (FST_RDB(card, interruptHandshake) == 1) {
		do_card_interrupt += FST_CARD_INT;
		 
		FST_WRB(card, interruptHandshake, 0xEE);
	}
	if (card->family == FST_FAMILY_TXU) {
		 
		dma_intcsr = inl(card->pci_conf + INTCSR_9054);
		if (dma_intcsr & 0x00200000) {
			 
			dbg(DBG_RX, "DMA Rx xfer complete\n");
			outb(0x8, card->pci_conf + DMACSR0);
			fst_rx_dma_complete(card, card->dma_port_rx,
					    card->dma_len_rx, card->dma_skb_rx,
					    card->dma_rxpos);
			card->dmarx_in_progress = 0;
			do_card_interrupt += FST_RX_DMA_INT;
		}
		if (dma_intcsr & 0x00400000) {
			 
			dbg(DBG_TX, "DMA Tx xfer complete\n");
			outb(0x8, card->pci_conf + DMACSR1);
			fst_tx_dma_complete(card, card->dma_port_tx,
					    card->dma_len_tx, card->dma_txpos);
			card->dmatx_in_progress = 0;
			do_card_interrupt += FST_TX_DMA_INT;
		}
	}

	 
	int_retry_count = FST_RDL(card, interruptRetryCount);
	if (int_retry_count) {
		dbg(DBG_ASS, "Card %d int_retry_count is  %d\n",
		    card->card_no, int_retry_count);
		FST_WRL(card, interruptRetryCount, 0);
	}

	if (!do_card_interrupt) {
		return IRQ_HANDLED;
	}

	 
	fst_q_work_item(&fst_work_intq, card->card_no);
	tasklet_schedule(&fst_int_task);

	 
	rdidx = FST_RDB(card, interruptEvent.rdindex) & 0x1f;
	wridx = FST_RDB(card, interruptEvent.wrindex) & 0x1f;
	while (rdidx != wridx) {
		event = FST_RDB(card, interruptEvent.evntbuff[rdidx]);
		port = &card->ports[event & 0x03];

		dbg(DBG_INTR, "Processing Interrupt event: %x\n", event);

		switch (event) {
		case TE1_ALMA:
			dbg(DBG_INTR, "TE1 Alarm intr\n");
			if (port->run)
				fst_intr_te1_alarm(card, port);
			break;

		case CTLA_CHG:
		case CTLB_CHG:
		case CTLC_CHG:
		case CTLD_CHG:
			if (port->run)
				fst_intr_ctlchg(card, port);
			break;

		case ABTA_SENT:
		case ABTB_SENT:
		case ABTC_SENT:
		case ABTD_SENT:
			dbg(DBG_TX, "Abort complete port %d\n", port->index);
			break;

		case TXA_UNDF:
		case TXB_UNDF:
		case TXC_UNDF:
		case TXD_UNDF:
			 
			dbg(DBG_TX, "Tx underflow port %d\n", port->index);
			port_to_dev(port)->stats.tx_errors++;
			port_to_dev(port)->stats.tx_fifo_errors++;
			dbg(DBG_ASS, "Tx underflow on card %d port %d\n",
			    card->card_no, port->index);
			break;

		case INIT_CPLT:
			dbg(DBG_INIT, "Card init OK intr\n");
			break;

		case INIT_FAIL:
			dbg(DBG_INIT, "Card init FAILED intr\n");
			card->state = FST_IFAILED;
			break;

		default:
			pr_err("intr: unknown card event %d. ignored\n", event);
			break;
		}

		 
		if (++rdidx >= MAX_CIRBUFF)
			rdidx = 0;
	}
	FST_WRB(card, interruptEvent.rdindex, rdidx);
        return IRQ_HANDLED;
}