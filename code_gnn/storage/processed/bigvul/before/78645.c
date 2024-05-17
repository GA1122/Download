static int piv_match_card(sc_card_t *card)
{
	int r = 0;

	 
	 
	switch (card->type) {
		case -1:
		case SC_CARD_TYPE_PIV_II_GENERIC:
		case SC_CARD_TYPE_PIV_II_HIST:
		case SC_CARD_TYPE_PIV_II_NEO:
		case SC_CARD_TYPE_PIV_II_YUBIKEY4:
		case SC_CARD_TYPE_PIV_II_GI_DE:
			break;
		default:
			return 0;  
	}
	 
	 
	r = piv_match_card_continued(card);
	if (r == 1) {
		 
		sc_unlock(card);
		piv_finish(card);
	}

	return r;
}
