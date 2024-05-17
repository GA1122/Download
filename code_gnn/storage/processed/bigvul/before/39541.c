fst_tx_dma(struct fst_card_info *card, unsigned char *skb,
	   unsigned char *mem, int len)
{
	 

	dbg(DBG_TX, "In fst_tx_dma %p %p %d\n", skb, mem, len);
	if (card->dmatx_in_progress) {
		dbg(DBG_ASS, "In fst_tx_dma while dma in progress\n");
	}

	outl((unsigned long) skb, card->pci_conf + DMAPADR1);	 
	outl((unsigned long) mem, card->pci_conf + DMALADR1);	 
	outl(len, card->pci_conf + DMASIZ1);	 
	outl(0x000000004, card->pci_conf + DMADPR1);	 

	 
	card->dmatx_in_progress = 1;
	outb(0x03, card->pci_conf + DMACSR1);	 
}
