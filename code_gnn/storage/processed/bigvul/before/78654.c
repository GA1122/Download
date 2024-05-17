static int piv_select_aid(sc_card_t* card, u8* aid, size_t aidlen, u8* response, size_t *responselen)
{
	sc_apdu_t apdu;
	int r;

	LOG_FUNC_CALLED(card->ctx);
	sc_log(card->ctx,
	       "Got args: aid=%p, aidlen=%"SC_FORMAT_LEN_SIZE_T"u, response=%p, responselen=%"SC_FORMAT_LEN_SIZE_T"u",
	       aid, aidlen, response, responselen ? *responselen : 0);

	sc_format_apdu(card, &apdu,
		response == NULL ? SC_APDU_CASE_3_SHORT : SC_APDU_CASE_4_SHORT, 0xA4, 0x04, 0x00);
	apdu.lc = aidlen;
	apdu.data = aid;
	apdu.datalen = aidlen;
	apdu.resp = response;
	apdu.resplen = responselen ? *responselen : 0;
	apdu.le = response == NULL ? 0 : 256;  

	r = sc_transmit_apdu(card, &apdu);
	if (responselen)
		*responselen = apdu.resplen;
	LOG_TEST_RET(card->ctx, r, "PIV select failed");

	LOG_FUNC_RETURN(card->ctx, sc_check_sw(card, apdu.sw1, apdu.sw2));
}