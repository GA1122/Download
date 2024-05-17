static int asepcos_select_file(sc_card_t *card, const sc_path_t *in_path,
	sc_file_t **file)
{
	int       r;
	sc_path_t npath = *in_path;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_NORMAL);

	if (in_path->type == SC_PATH_TYPE_PATH) {
		 
		sc_path_t tpath;

		r = asepcos_get_current_df_path(card, &tpath);
		 
		if (r != SC_ERROR_INVALID_ASN1_OBJECT && r != SC_SUCCESS)
			return r;
		if (r == SC_SUCCESS && sc_compare_path_prefix(&tpath, &npath) != 0) {
			 
			if (tpath.len == npath.len) {
				 
				if (file == NULL)
					 
					return SC_SUCCESS;
			} else {
				 
				r = sc_path_set(&npath, 0, &in_path->value[tpath.len], 
						npath.len - tpath.len, 0, 0);
				if (r != SC_SUCCESS)
					return r;
				if (npath.len == 2)
					npath.type = SC_PATH_TYPE_FILE_ID;
				else
					npath.type = SC_PATH_TYPE_PATH;
			}
		}
	}

	r = iso_ops->select_file(card, &npath, file);
	 
	if (file != NULL && *file != NULL) 
		if ((*file)->ef_structure == SC_FILE_EF_UNKNOWN)
			(*file)->ef_structure = SC_FILE_EF_TRANSPARENT;
	if (r == SC_SUCCESS && file != NULL && *file != NULL) {
		r = asepcos_parse_sec_attr(card, *file, (*file)->sec_attr, (*file)->sec_attr_len);
		if (r != SC_SUCCESS) 
			sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "error parsing security attributes");
	}
	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, r);
}