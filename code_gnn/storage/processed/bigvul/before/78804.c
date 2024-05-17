static int read_file(sc_pkcs15_card_t * p15card, u8 fid[2],
		u8 *efbin, size_t *len, int optional)
{
	sc_path_t path;
	int r;

	sc_path_set(&path, SC_PATH_TYPE_FILE_ID, fid, 2, 0, 0);
	 
	path.aid = sc_hsm_aid;
	 
	path.count = -1;
	if (!p15card->opts.use_file_cache || !efbin
			|| SC_SUCCESS != sc_pkcs15_read_cached_file(p15card, &path, &efbin, len)) {
		 
		path.aid.len = 0;
		r = sc_select_file(p15card->card, &path, NULL);
		if (r < 0) {
			sc_log(p15card->card->ctx, "Could not select EF");
		} else {
			r = sc_read_binary(p15card->card, 0, efbin, *len, 0);
		}

		if (r < 0) {
			sc_log(p15card->card->ctx, "Could not read EF");
			if (!optional) {
				return r;
			}
			 
			*len = 0;
		} else {
			*len = r;
		}

		if (p15card->opts.use_file_cache) {
			 
			path.aid = sc_hsm_aid;
			sc_pkcs15_cache_file(p15card, &path, efbin, *len);
		}
	}

	return SC_SUCCESS;
}
