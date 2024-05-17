auth_delete_file(struct sc_card *card, const struct sc_path *path)
{
	struct sc_apdu apdu;
	unsigned char sbuf[2];
	int rv;
	char pbuf[SC_MAX_PATH_STRING_SIZE];

	LOG_FUNC_CALLED(card->ctx);

	rv = sc_path_print(pbuf, sizeof(pbuf), path);
	if (rv != SC_SUCCESS)
		pbuf[0] = '\0';

	sc_log(card->ctx, "path; type=%d, path=%s", path->type, pbuf);

	if (path->len < 2)   {
		sc_log(card->ctx, "Invalid path length");
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_ARGUMENTS);
	}

	if (path->len > 2)   {
		struct sc_path parent = *path;

		parent.len -= 2;
		parent.type = SC_PATH_TYPE_PATH;
		rv = auth_select_file(card, &parent, NULL);
		LOG_TEST_RET(card->ctx, rv, "select parent failed ");
	}

	sbuf[0] = path->value[path->len - 2];
	sbuf[1] = path->value[path->len - 1];

	if (memcmp(sbuf,"\x00\x00",2)==0 || (memcmp(sbuf,"\xFF\xFF",2)==0) ||
			memcmp(sbuf,"\x3F\xFF",2)==0)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INCORRECT_PARAMETERS);

	sc_format_apdu(card, &apdu, SC_APDU_CASE_3_SHORT, 0xE4, 0x02, 0x00);
	apdu.lc = 2;
	apdu.datalen = 2;
	apdu.data = sbuf;

	rv = sc_transmit_apdu(card, &apdu);
	LOG_TEST_RET(card->ctx, rv, "APDU transmit failed");
	if (apdu.sw1==0x6A && apdu.sw2==0x82)   {
		 
		struct sc_path tmp_path;
		int ii, len;
		unsigned char lbuf[SC_MAX_APDU_BUFFER_SIZE];

		memset(&tmp_path, 0, sizeof(struct sc_path));
		tmp_path.type = SC_PATH_TYPE_FILE_ID;
		memcpy(tmp_path.value, sbuf, 2);
		tmp_path.len = 2;
		rv = auth_select_file(card, &tmp_path, NULL);
		LOG_TEST_RET(card->ctx, rv, "select DF failed");

		len = auth_list_files(card, lbuf, sizeof(lbuf));
		LOG_TEST_RET(card->ctx, len, "list DF failed");

		for (ii=0; ii<len/2; ii++)   {
			struct sc_path tmp_path_x;

			memset(&tmp_path_x, 0, sizeof(struct sc_path));
			tmp_path_x.type = SC_PATH_TYPE_FILE_ID;
			tmp_path_x.value[0] = *(lbuf + ii*2);
			tmp_path_x.value[1] = *(lbuf + ii*2 + 1);
			tmp_path_x.len = 2;

			rv = auth_delete_file(card, &tmp_path_x);
			LOG_TEST_RET(card->ctx, rv, "delete failed");
		}

		tmp_path.type = SC_PATH_TYPE_PARENT;
		rv = auth_select_file(card, &tmp_path, NULL);
		LOG_TEST_RET(card->ctx, rv, "select parent failed");

		apdu.p1 = 1;
		rv = sc_transmit_apdu(card, &apdu);
	}

	LOG_TEST_RET(card->ctx, rv, "APDU transmit failed");
	rv = sc_check_sw(card, apdu.sw1, apdu.sw2);

	LOG_FUNC_RETURN(card->ctx, rv);
}