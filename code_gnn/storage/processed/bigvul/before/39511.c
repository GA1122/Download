fst_cpurelease(struct fst_card_info *card)
{
	if (card->family == FST_FAMILY_TXU) {
		 
		(void) readb(card->mem);

		 
		outw(0x040e, card->pci_conf + CNTRL_9054 + 2);
		outw(0x040f, card->pci_conf + CNTRL_9054 + 2);
	} else {
		(void) readb(card->ctlmem);
	}
}
