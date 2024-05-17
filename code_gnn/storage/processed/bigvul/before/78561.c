auth_select_aid(struct sc_card *card)
{
	struct sc_apdu apdu;
	unsigned char apdu_resp[SC_MAX_APDU_BUFFER_SIZE];
	struct auth_private_data *data =  (struct auth_private_data *) card->drv_data;
	int rv, ii;
	struct sc_path tmp_path;

	 
	rv = gp_select_card_manager(card);
	LOG_TEST_RET(card->ctx, rv, "APDU transmit failed");

	 
	sc_format_apdu(card, &apdu, SC_APDU_CASE_2_SHORT, 0xCA, 0x9F, 0x7F);
	apdu.cla = 0x80;
	apdu.le = 0x2D;
	apdu.resplen = 0x30;
	apdu.resp = apdu_resp;

	rv = sc_transmit_apdu(card, &apdu);
	LOG_TEST_RET(card->ctx, rv, "APDU transmit failed");
	card->serialnr.len = 4;
	memcpy(card->serialnr.value, apdu.resp+15, 4);

	for (ii=0, data->sn = 0; ii < 4; ii++)
		data->sn += (long int)(*(apdu.resp + 15 + ii)) << (3-ii)*8;

	sc_log(card->ctx, "serial number %li/0x%lX", data->sn, data->sn);

	memset(&tmp_path, 0, sizeof(struct sc_path));
	tmp_path.type = SC_PATH_TYPE_DF_NAME;
	memcpy(tmp_path.value, aidAuthentIC_V5, lenAidAuthentIC_V5);
	tmp_path.len = lenAidAuthentIC_V5;

	rv = iso_ops->select_file(card, &tmp_path, NULL);
	LOG_TEST_RET(card->ctx, rv, "select parent failed");

	sc_format_path("3F00", &tmp_path);
	rv = iso_ops->select_file(card, &tmp_path, &auth_current_df);
	LOG_TEST_RET(card->ctx, rv, "select parent failed");

	sc_format_path("3F00", &card->cache.current_path);
	sc_file_dup(&auth_current_ef, auth_current_df);

	memcpy(data->aid, aidAuthentIC_V5, lenAidAuthentIC_V5);
	data->aid_len = lenAidAuthentIC_V5;
	card->name = nameAidAuthentIC_V5;

	LOG_FUNC_RETURN(card->ctx, rv);
}