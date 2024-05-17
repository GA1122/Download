gpk_pkfile_load(sc_card_t *card, struct sc_cardctl_gpk_pkload *args)
{
	struct gpk_private_data *priv = DRVDATA(card);
	sc_apdu_t	apdu;
	unsigned int	n;
	u8		temp[256];
	int		r = SC_SUCCESS, outl;
	EVP_CIPHER_CTX  * ctx;

	sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "gpk_pkfile_load(fid=%04x, len=%d, datalen=%d)\n",
			args->file->id, args->len, args->datalen);

	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL)
		return SC_ERROR_INTERNAL;

	if (0) {
		sc_log_hex(card->ctx, "Sending (cleartext)",
				args->data, args->datalen);
	}

	memset(&apdu, 0, sizeof(apdu));
	apdu.cse = SC_APDU_CASE_3_SHORT;
	apdu.cla = 0x80;
	apdu.ins = 0x18;
	apdu.p1  = args->file->id & 0x1F;
	apdu.p2  = args->len;
	apdu.lc  = args->datalen;

	 
	assert(args->datalen <= sizeof(temp));
	if (!priv->key_set) {
		sc_debug(card->ctx, SC_LOG_DEBUG_NORMAL, "No secure messaging key set!\n");
		return SC_ERROR_SECURITY_STATUS_NOT_SATISFIED;
	}

	EVP_EncryptInit_ex(ctx, EVP_des_ede(), NULL, priv->key, NULL);
	for (n = 0; n < args->datalen; n += 8) {
		if (!EVP_EncryptUpdate(ctx, temp+n, &outl, args->data + n, 8)) {
			r = SC_ERROR_INTERNAL;
			break;
		}
	}
	if (ctx)
		EVP_CIPHER_CTX_free(ctx);
	if (r != SC_SUCCESS)
		return SC_ERROR_INTERNAL;

	apdu.data = temp;
	apdu.datalen = args->datalen;

	 
	priv->key_set = 0;

	r = sc_transmit_apdu(card, &apdu);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "APDU transmit failed");
	r = sc_check_sw(card, apdu.sw1, apdu.sw2);
	SC_TEST_RET(card->ctx, SC_LOG_DEBUG_NORMAL, r, "Card returned error");

	SC_FUNC_RETURN(card->ctx, SC_LOG_DEBUG_NORMAL, r);
}