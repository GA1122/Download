static int piv_process_discovery(sc_card_t *card)
{
	int r;
	u8 * rbuf = NULL;
	size_t rbuflen = 0;

	r = piv_get_cached_data(card, PIV_OBJ_DISCOVERY, &rbuf, &rbuflen);
	 
	if (r < 0)
		goto err;

	sc_log(card->ctx, "Discovery = %p:%"SC_FORMAT_LEN_SIZE_T"u", rbuf,
	       rbuflen);
	 
	r = piv_parse_discovery(card, rbuf, rbuflen, 0);

err:
	LOG_FUNC_RETURN(card->ctx, r);
}