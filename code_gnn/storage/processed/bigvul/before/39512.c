fst_cpureset(struct fst_card_info *card)
{
	unsigned char interrupt_line_register;
	unsigned long j = jiffies + 1;
	unsigned int regval;

	if (card->family == FST_FAMILY_TXU) {
		if (pci_read_config_byte
		    (card->device, PCI_INTERRUPT_LINE, &interrupt_line_register)) {
			dbg(DBG_ASS,
			    "Error in reading interrupt line register\n");
		}
		 
		outw(0x440f, card->pci_conf + CNTRL_9054 + 2);
		outw(0x040f, card->pci_conf + CNTRL_9054 + 2);
		 
		j = jiffies + 1;
		while (jiffies < j)
			  ;
		outw(0x240f, card->pci_conf + CNTRL_9054 + 2);
		 
		j = jiffies + 1;
		while (jiffies < j)
			  ;
		outw(0x040f, card->pci_conf + CNTRL_9054 + 2);

		if (pci_write_config_byte
		    (card->device, PCI_INTERRUPT_LINE, interrupt_line_register)) {
			dbg(DBG_ASS,
			    "Error in writing interrupt line register\n");
		}

	} else {
		regval = inl(card->pci_conf + CNTRL_9052);

		outl(regval | 0x40000000, card->pci_conf + CNTRL_9052);
		outl(regval & ~0x40000000, card->pci_conf + CNTRL_9052);
	}
}