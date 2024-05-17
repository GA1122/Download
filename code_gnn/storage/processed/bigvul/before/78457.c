gpk_select_id(sc_card_t *card, int kind, unsigned int fid,
		sc_file_t **file)
{
	sc_path_t	*cp = &card->cache.current_path;
	u8		fbuf[2];
	int		r;

	sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL,
		"gpk_select_id(0x%04X, kind=%u)\n", fid, kind);

	fbuf[0] = fid >> 8;
	fbuf[1] = fid & 0xff;

	r = gpk_select(card, kind, fbuf, 2, file);

	 
	if (r == 0) {
		unsigned short int	*path;

		switch (kind) {
		case GPK_SEL_MF:
			cp->len = 0;
			 
		case GPK_SEL_DF:
			assert(cp->len + 1 <= SC_MAX_PATH_SIZE / 2);
			path = (unsigned short int *) cp->value;
			path[cp->len++] = fid;
		}
	} else {
		cp->len = 0;
	}
	return r;
}