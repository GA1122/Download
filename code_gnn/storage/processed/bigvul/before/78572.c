gnuk_delete_key(sc_card_t *card, u8 key_id)
{
	sc_context_t *ctx = card->ctx;
	int r = SC_SUCCESS;
	char *data = NULL;

	LOG_FUNC_CALLED(ctx);

	if (key_id < 1 || key_id > 3) {
		sc_log(ctx, "Key ID %d is invalid. Should be 1, 2 or 3.", key_id);
		LOG_FUNC_RETURN(ctx, SC_ERROR_INVALID_ARGUMENTS);
	}

	 
	sc_log(ctx, "Delete fingerprints");
	r = pgp_put_data(card, 0xC6 + key_id, NULL, 0);
	LOG_TEST_RET(ctx, r, "Failed to delete fingerprints");
	 
	sc_log(ctx, "Delete creation time");
	r = pgp_put_data(card, 0xCD + key_id, NULL, 0);
	LOG_TEST_RET(ctx, r, "Failed to delete creation time");

	 
	sc_log(ctx, "Rewrite Extended Header List");

	if (key_id == 1)
		data = "\x4D\x02\xB6";
	else if (key_id == 2)
		data = "\x4D\x02\xB8";
	else if (key_id == 3)
		data = "\x4D\x02\xA4";

	r = pgp_put_data(card, 0x4D, (const u8 *)data, strlen((const char *)data) + 1);

	LOG_FUNC_RETURN(ctx, r);
}