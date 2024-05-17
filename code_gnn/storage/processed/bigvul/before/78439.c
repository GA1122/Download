gpk_erase_card(sc_card_t *card)
{
	struct gpk_private_data *priv = DRVDATA(card);
	sc_apdu_t	apdu;
	u8		offset;
	int		r;

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);
	switch (card->type) {
	case SC_CARD_TYPE_GPK_GPK4000_su256:
	case SC_CARD_TYPE_GPK_GPK4000_sdo:
		offset = 0x6B;   
		break;

	case SC_CARD_TYPE_GPK_GPK4000_s:
		offset = 7;
		break;

	case SC_CARD_TYPE_GPK_GPK8000:
	case SC_CARD_TYPE_GPK_GPK8000_8K:
	case SC_CARD_TYPE_GPK_GPK8000_16K:
	case SC_CARD_TYPE_GPK_GPK16000:
		offset = 0;
		break;

	default:
		return SC_ERROR_NOT_SUPPORTED;
	}

	memset(&apdu, 0, sizeof(apdu));
	apdu.cse = SC_APDU_CASE_1;
	apdu.cla = 0xDB;
	apdu.ins = 0xDE;
	apdu.p2  = offset;

	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Card returned error");

	priv->key_set = 0;
	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_VERBOSE, r);
}