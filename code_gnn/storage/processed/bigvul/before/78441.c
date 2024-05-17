gpk_generate_key(sc_card_t *card, struct sc_cardctl_gpk_genkey *args)
{
	sc_apdu_t	apdu;
	int		r;
	u8		buffer[256];

	sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL,
		"gpk_generate_key(%u)\n", args->privlen);
	if (args->privlen != 512 && args->privlen != 1024) {
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL,
			"Key generation not supported for key length %d",
			args->privlen);
		return SC_ERROR_NOT_SUPPORTED;
	}

	memset(&apdu, 0, sizeof(apdu));
	apdu.cse = SC_APDU_CASE_2_SHORT;
	apdu.cla = 0x80;
	apdu.ins = 0xD2;
	apdu.p1  = 0x80 | (args->fid & 0x1F);
	apdu.p2  = (args->privlen == 1024) ? 0x11 : 0;
	apdu.le  = args->privlen / 8 + 2;
	apdu.resp = buffer;
	apdu.resplen = 256;

	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Card returned error");

	 
	if (args->pubkey_len && apdu.resplen > 2) {
		r = reverse(args->pubkey, args->pubkey_len,
				buffer + 2, apdu.resplen - 2);
		SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Failed to reverse buffer");
		args->pubkey_len = r;
	}

	return r;
}