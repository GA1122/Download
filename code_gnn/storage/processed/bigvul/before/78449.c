gpk_match_card(sc_card_t *card)
{
	int i;

	i = _sc_match_atr(card, gpk_atrs, &card->type);
	if (i < 0) {
		const u8 *hist_bytes = card->reader->atr_info.hist_bytes;

		 

		if ((card->reader->atr_info.hist_bytes_len >= 7)
			&& (hist_bytes[0] == 0x80)
			&& (hist_bytes[1] == 0x65)
			&& (hist_bytes[2] == 0xa2)) {	 
			if (hist_bytes[3] == 0x08) {	 
				card->type = SC_CARD_TYPE_GPK_GPK8000;
				return 1;
			}
			if (hist_bytes[3] == 0x09) {	 
				card->type = SC_CARD_TYPE_GPK_GPK16000;
				return 1;
			}
		}
		return 0;
	}
	return 1;
}