fst_rx_dma(struct fst_card_info *card, dma_addr_t skb,
	   dma_addr_t mem, int len)
{
	 

	dbg(DBG_RX, "In fst_rx_dma %lx %lx %d\n",
	    (unsigned long) skb, (unsigned long) mem, len);
	if (card->dmarx_in_progress) {
		dbg(DBG_ASS, "In fst_rx_dma while dma in progress\n");
	}

	outl(skb, card->pci_conf + DMAPADR0);	 
	outl(mem, card->pci_conf + DMALADR0);	 
	outl(len, card->pci_conf + DMASIZ0);	 
	outl(0x00000000c, card->pci_conf + DMADPR0);	 

	 
	card->dmarx_in_progress = 1;
	outb(0x03, card->pci_conf + DMACSR0);	 
}
