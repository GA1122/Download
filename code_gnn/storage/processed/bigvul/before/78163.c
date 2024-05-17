static int asepcos_list_files(sc_card_t *card, u8 *buf, size_t blen)
{
	int       r, rv = 0, dfFID, efFID;
	sc_path_t bpath, tpath;
	sc_file_t *tfile = NULL;

	 
	r = asepcos_get_current_df_path(card, &bpath);
	if (r != SC_SUCCESS)
		return r;
	 
	r = sc_select_file(card, &bpath, &tfile);
	if (r != SC_SUCCESS)
		return r;
	if (tfile->prop_attr_len != 6 || tfile->prop_attr == NULL) {
		sc_file_free(tfile);
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "unable to parse proprietary FCI attributes");
		return SC_ERROR_INTERNAL;
	}
	dfFID = (tfile->prop_attr[2] << 8) | tfile->prop_attr[3];
	efFID = (tfile->prop_attr[4] << 8) | tfile->prop_attr[5];
	sc_file_free(tfile);
	 
	while (dfFID != 0) {
		 
		if (blen < 2)
			return SC_ERROR_BUFFER_TOO_SMALL;
		*buf++ = (dfFID >> 8) & 0xff;
		*buf++ = dfFID & 0xff;
		rv   += 2;
		blen -= 2;
		 
		tpath = bpath;
		r = sc_append_file_id(&tpath, dfFID);
		if (r != SC_SUCCESS)
			return r;
		r = sc_select_file(card, &tpath, &tfile);
		if (r != SC_SUCCESS)
			return r;
		if (tfile->prop_attr_len != 6 || tfile->prop_attr == NULL)
			return SC_ERROR_INTERNAL;
		dfFID = (tfile->prop_attr[0] << 8) | tfile->prop_attr[1];
		sc_file_free(tfile);
	}
	 
	while (efFID != 0) {
		 
		if (blen < 2)
			return SC_ERROR_BUFFER_TOO_SMALL;
		*buf++ = (efFID >> 8) & 0xff;
		*buf++ = efFID & 0xff;
		rv   += 2;
		blen -= 2;
		 
		tpath = bpath;
		r = sc_append_file_id(&tpath, efFID);
		if (r != SC_SUCCESS)
			return r;
		r = sc_select_file(card, &tpath, &tfile);
		if (r != SC_SUCCESS)
			return r;
		if (tfile->prop_attr_len < 2 || tfile->prop_attr == NULL)
			return SC_ERROR_INTERNAL;
		efFID = (tfile->prop_attr[0] << 8) | tfile->prop_attr[1];
		sc_file_free(tfile);
	}

	return rv;
}