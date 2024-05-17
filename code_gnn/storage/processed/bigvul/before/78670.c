static int rtecp_decipher(sc_card_t *card,
		const u8 *data, size_t data_len, u8 *out, size_t out_len)
{
	int r;

	assert(card && card->ctx && data && out);
	 
	r = rtecp_cipher(card, data, data_len, out, out_len, 0);
	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, r);
}