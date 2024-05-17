pgp_store_creationtime(sc_card_t *card, u8 key_id, time_t *outtime)
{
	int r;
	time_t createtime = 0;
	const size_t timestrlen = 64;
	char timestring[65];
	u8 buf[4];

	LOG_FUNC_CALLED(card->ctx);
	if (key_id == 0 || key_id > 3) {
		sc_log(card->ctx, "Invalid key ID %d.", key_id);
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_DATA);
	}

	if (outtime != NULL && *outtime != 0)
		createtime = *outtime;
	else if (outtime != NULL)
		 
		*outtime = createtime = time(NULL);

	strftime(timestring, timestrlen, "%c %Z", gmtime(&createtime));
	sc_log(card->ctx, "Creation time %s.", timestring);
	 
	ulong2bebytes(buf, (unsigned long)createtime);
	r = pgp_put_data(card, 0x00CD + key_id, buf, 4);
	LOG_TEST_RET(card->ctx, r, "Cannot write to DO");
	LOG_FUNC_RETURN(card->ctx, r);
}