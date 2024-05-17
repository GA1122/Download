static int coolkey_check_sw(sc_card_t *card, unsigned int sw1, unsigned int sw2)
{
	sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL,
		"sw1 = 0x%02x, sw2 = 0x%02x\n", sw1, sw2);

	if (sw1 == 0x90)
		return SC_SUCCESS;

	if (sw1 == 0x9c) {
		if (sw2 == 0xff) {
			 
			return SC_ERROR_INTERNAL;
		}
		if (sw2 >= coolkey_number_of_error_codes) {
			return SC_ERROR_UNKNOWN;
		}
		return coolkey_error_codes[sw2].sc_error;
	}

	 
        return sc_get_iso7816_driver()->ops->check_sw(card, sw1, sw2);
}
