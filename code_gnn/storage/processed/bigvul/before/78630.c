static int piv_find_discovery(sc_card_t *card)
{
	int r = 0;
	u8  rbuf[256];
	size_t rbuflen = sizeof(rbuf);
	u8 * arbuf = rbuf;
	piv_private_data_t * priv = PIV_DATA(card);

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	 

	 
	if (!(priv->obj_cache[PIV_OBJ_DISCOVERY].flags & PIV_OBJ_CACHE_VALID)) {
		r = piv_process_discovery(card);
	} else {
		 
		r = piv_get_data(card, PIV_OBJ_DISCOVERY, &arbuf, &rbuflen);
		if (r >= 0)
			 
			r = piv_parse_discovery(card, rbuf, rbuflen, 1);
	}

	LOG_FUNC_RETURN(card->ctx, r);
}
