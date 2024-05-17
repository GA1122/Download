auth_create_file(struct sc_card *card, struct sc_file *file)
{
	struct sc_apdu apdu;
	struct sc_path path;
	int rv, rec_nr;
	unsigned char sbuf[0x18];
	size_t sendlen = sizeof(sbuf);
	char pbuf[SC_MAX_PATH_STRING_SIZE];

	LOG_FUNC_CALLED(card->ctx);

	rv = sc_path_print(pbuf, sizeof(pbuf), &file->path);
	if (rv != SC_SUCCESS)
		pbuf[0] = '\0';
	sc_log(card->ctx, " create path=%s", pbuf);

	sc_log(card->ctx,
	       "id %04X; size %"SC_FORMAT_LEN_SIZE_T"u; type 0x%X; ef 0x%X",
	       file->id, file->size, file->type, file->ef_structure);

	if (file->id==0x0000 || file->id==0xFFFF || file->id==0x3FFF)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_ARGUMENTS);

	rv = sc_path_print(pbuf, sizeof(pbuf), &card->cache.current_path);
	if (rv != SC_SUCCESS)
		pbuf[0] = '\0';

	if (file->path.len)   {
		memcpy(&path, &file->path, sizeof(path));
		if (path.len>2)
			path.len -= 2;

		if (auth_select_file(card, &path, NULL))   {
			sc_log(card->ctx, "Cannot select parent DF.");
			LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_ARGUMENTS);
		}
	}

	rv = encode_file_structure_V5(card, file, sbuf, &sendlen);
	LOG_TEST_RET(card->ctx, rv, "File structure encoding failed");

	if (file->type != SC_FILE_TYPE_DF && file->ef_structure != SC_FILE_EF_TRANSPARENT)
		rec_nr = file->record_count;
	else
		rec_nr = 0;

	sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT, 0xE0, 0x00, rec_nr);
	apdu.data = sbuf;
	apdu.datalen = sendlen;
	apdu.lc = sendlen;

	rv = sc_transmit_apdu(card, &apdu);
	LOG_TEST_RET(card->ctx, rv, "APDU transmit failed");
	rv = sc_check_sw(card, apdu.sw1, apdu.sw2);
	LOG_TEST_RET(card->ctx, rv, "Card returned error");

	 
	if (file->type == SC_FILE_TYPE_DF)   {
		struct sc_path tmp_path;
		struct sc_file *df_file = NULL;

		memset(&tmp_path, 0, sizeof(struct sc_path));
		tmp_path.type = SC_PATH_TYPE_FILE_ID;
		tmp_path.value[0] = file->id >> 8;
		tmp_path.value[1] = file->id & 0xFF;
		tmp_path.len = 2;
		rv = auth_select_file(card, &tmp_path, &df_file);
		sc_log(card->ctx, "rv %i", rv);
	}

	sc_file_free(auth_current_ef);
	sc_file_dup(&auth_current_ef, file);

	LOG_FUNC_RETURN(card->ctx, rv);
}