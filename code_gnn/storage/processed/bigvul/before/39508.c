fst_clear_intr(struct fst_card_info *card)
{
	if (card->family == FST_FAMILY_TXU) {
		(void) readb(card->ctlmem);
	} else {
		 
		outw(0x0543, card->pci_conf + INTCSR_9052);
	}
}
