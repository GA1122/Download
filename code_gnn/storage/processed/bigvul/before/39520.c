fst_intr_rx(struct fst_card_info *card, struct fst_port_info *port)
{
	unsigned char dmabits;
	int pi;
	int rxp;
	int rx_status;
	unsigned short len;
	struct sk_buff *skb;
	struct net_device *dev = port_to_dev(port);

	 
	pi = port->index;
	rxp = port->rxpos;
	dmabits = FST_RDB(card, rxDescrRing[pi][rxp].bits);
	if (dmabits & DMA_OWN) {
		dbg(DBG_RX | DBG_INTR, "intr_rx: No buffer port %d pos %d\n",
		    pi, rxp);
		return;
	}
	if (card->dmarx_in_progress) {
		return;
	}

	 
	len = FST_RDW(card, rxDescrRing[pi][rxp].mcnt);
	 
	len -= 2;
	if (len == 0) {
		 
		pr_err("Frame received with 0 length. Card %d Port %d\n",
		       card->card_no, port->index);
		 
		FST_WRB(card, rxDescrRing[pi][rxp].bits, DMA_OWN);

		rxp = (rxp+1) % NUM_RX_BUFFER;
		port->rxpos = rxp;
		return;
	}

	 
	dbg(DBG_RX, "intr_rx: %d,%d: flags %x len %d\n", pi, rxp, dmabits, len);
	if (dmabits != (RX_STP | RX_ENP) || len > LEN_RX_BUFFER - 2) {
		fst_log_rx_error(card, port, dmabits, rxp, len);
		fst_recover_rx_error(card, port, dmabits, rxp, len);
		return;
	}

	 
	if ((skb = dev_alloc_skb(len)) == NULL) {
		dbg(DBG_RX, "intr_rx: can't allocate buffer\n");

		dev->stats.rx_dropped++;

		 
		FST_WRB(card, rxDescrRing[pi][rxp].bits, DMA_OWN);

		rxp = (rxp+1) % NUM_RX_BUFFER;
		port->rxpos = rxp;
		return;
	}

	 

	if ((len < FST_MIN_DMA_LEN) || (card->family == FST_FAMILY_TXP)) {
		memcpy_fromio(skb_put(skb, len),
			      card->mem + BUF_OFFSET(rxBuffer[pi][rxp][0]),
			      len);

		 
		FST_WRB(card, rxDescrRing[pi][rxp].bits, DMA_OWN);

		 
		dev->stats.rx_packets++;
		dev->stats.rx_bytes += len;

		 
		dbg(DBG_RX, "Pushing frame up the stack\n");
		if (port->mode == FST_RAW)
			skb->protocol = farsync_type_trans(skb, dev);
		else
			skb->protocol = hdlc_type_trans(skb, dev);
		rx_status = netif_rx(skb);
		fst_process_rx_status(rx_status, port_to_dev(port)->name);
		if (rx_status == NET_RX_DROP)
			dev->stats.rx_dropped++;
	} else {
		card->dma_skb_rx = skb;
		card->dma_port_rx = port;
		card->dma_len_rx = len;
		card->dma_rxpos = rxp;
		fst_rx_dma(card, card->rx_dma_handle_card,
			   BUF_OFFSET(rxBuffer[pi][rxp][0]), len);
	}
	if (rxp != port->rxpos) {
		dbg(DBG_ASS, "About to increment rxpos by more than 1\n");
		dbg(DBG_ASS, "rxp = %d rxpos = %d\n", rxp, port->rxpos);
	}
	rxp = (rxp+1) % NUM_RX_BUFFER;
	port->rxpos = rxp;
}
