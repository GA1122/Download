static int piv_init(sc_card_t *card)
{
	int r = 0;
	piv_private_data_t * priv = NULL;
	sc_apdu_t apdu;
	unsigned long flags;
	unsigned long ext_flags;
	u8 yubico_version_buf[3];

	SC_FUNC_CALLED(card->ctx, SC_LOG_DEBUG_VERBOSE);

	 
	r = piv_match_card_continued(card);
	if (r != 1)  {
		sc_log(card->ctx,"piv_match_card_continued failed");
		piv_finish(card);
		 
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_CARD);
	}
		
	priv = PIV_DATA(card);

	 
	if (!priv || card->type == -1)
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_CARD);

	sc_log(card->ctx,
	       "Max send = %"SC_FORMAT_LEN_SIZE_T"u recv = %"SC_FORMAT_LEN_SIZE_T"u card->type = %d",
	       card->max_send_size, card->max_recv_size, card->type);
	card->cla = 0x00;
	if(card->name == NULL)
		card->name = card->driver->name;

	 

	switch(card->type) {
		case SC_CARD_TYPE_PIV_II_NEO:
		case SC_CARD_TYPE_PIV_II_YUBIKEY4:
			sc_format_apdu(card, &apdu, SC_APDU_CASE_2_SHORT, 0xFD, 0x00, 0x00);
			apdu.lc = 0;
			apdu.data = NULL;
			apdu.datalen = 0;
			apdu.resp = yubico_version_buf;
			apdu.resplen = sizeof(yubico_version_buf);
			apdu.le = apdu.resplen;
			r = sc_transmit_apdu(card, &apdu);
			priv->yubico_version = (yubico_version_buf[0]<<16) | (yubico_version_buf[1] <<8) | yubico_version_buf[2];
			sc_log(card->ctx, "Yubico card->type=%d, r=0x%08x version=0x%08x", card->type, r, priv->yubico_version);
			break;
	}

	 

	switch(card->type) {
		case SC_CARD_TYPE_PIV_II_NEO:
			priv->card_issues |= CI_NO_EC384
				| CI_VERIFY_630X
				| CI_OTHER_AID_LOSE_STATE
				| CI_LEAKS_FILE_NOT_FOUND
				| CI_NFC_EXPOSE_TOO_MUCH;
			if (priv->yubico_version  < 0x00040302)
				priv->card_issues |= CI_VERIFY_LC0_FAIL;
			break;

		case SC_CARD_TYPE_PIV_II_YUBIKEY4:
			priv->card_issues |=  CI_OTHER_AID_LOSE_STATE
				| CI_LEAKS_FILE_NOT_FOUND;
			if (priv->yubico_version  < 0x00040302)
				priv->card_issues |= CI_VERIFY_LC0_FAIL;
			break;

		case SC_CARD_TYPE_PIV_II_HIST:
			priv->card_issues |= 0;
			break;

		case SC_CARD_TYPE_PIV_II_GI_DE:
			priv->card_issues |= CI_VERIFY_LC0_FAIL
				| CI_PIV_AID_LOSE_STATE
				| CI_OTHER_AID_LOSE_STATE;;
			 
			break;

		case SC_CARD_TYPE_PIV_II_GENERIC:
			priv->card_issues |= CI_VERIFY_LC0_FAIL
				| CI_OTHER_AID_LOSE_STATE;
			 
			break;

		default:
		     priv->card_issues = 0;  
		     sc_log(card->ctx, "Unknown PIV card->type %d", card->type);
		     card->type = SC_CARD_TYPE_PIV_II_BASE;
	}
	sc_log(card->ctx, "PIV card-type=%d card_issues=0x%08x", card->type, priv->card_issues);

	priv->enumtag = piv_aids[0].enumtag;

	 

	 flags = SC_ALGORITHM_RSA_RAW;

	_sc_card_add_rsa_alg(card, 1024, flags, 0);  
	_sc_card_add_rsa_alg(card, 2048, flags, 0);  
	_sc_card_add_rsa_alg(card, 3072, flags, 0);  

	flags = SC_ALGORITHM_ECDSA_RAW | SC_ALGORITHM_ECDH_CDH_RAW | SC_ALGORITHM_ECDSA_HASH_NONE;
	ext_flags = SC_ALGORITHM_EXT_EC_NAMEDCURVE | SC_ALGORITHM_EXT_EC_UNCOMPRESES;

	_sc_card_add_ec_alg(card, 256, flags, ext_flags, NULL);
	if (!(priv->card_issues & CI_NO_EC384))
		_sc_card_add_ec_alg(card, 384, flags, ext_flags, NULL);

	 
	card->caps |= SC_CARD_CAP_RNG | SC_CARD_CAP_ISO7816_PIN_INFO;

	 
	piv_process_history(card);

	piv_process_discovery(card);

	priv->pstate=PIV_STATE_NORMAL;
	sc_unlock(card) ;  
	LOG_FUNC_RETURN(card->ctx, SC_SUCCESS);
}
