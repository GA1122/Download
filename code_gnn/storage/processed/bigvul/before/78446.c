gpk_init(sc_card_t *card)
{
	struct gpk_private_data *priv;
	unsigned long	exponent, flags, kg;
	unsigned char info[13];

	card->drv_data = priv = calloc(1, sizeof(*priv));
	if (card->drv_data == NULL)
		return SC_ERROR_OUT_OF_MEMORY;

	 
	priv->offset_shift = 2;
	priv->offset_mask = 3;
	card->cla = 0x00;

	 
	flags = SC_ALGORITHM_RSA_HASH_MD5 | SC_ALGORITHM_RSA_HASH_SHA1
		| SC_ALGORITHM_RSA_HASH_MD5_SHA1;
	flags |= SC_ALGORITHM_RSA_PAD_PKCS1 | SC_ALGORITHM_RSA_PAD_ANSI
		| SC_ALGORITHM_RSA_PAD_ISO9796;
	exponent = (card->type < SC_CARD_TYPE_GPK_GPK16000) ? 0x10001 : 0;
	kg = (card->type >= SC_CARD_TYPE_GPK_GPK8000) ? SC_ALGORITHM_ONBOARD_KEY_GEN : 0;
	_sc_card_add_rsa_alg(card,  512, flags|kg, exponent);
	_sc_card_add_rsa_alg(card,  768, flags, exponent);
	_sc_card_add_rsa_alg(card, 1024, flags|kg, exponent);

	 
	if (gpk_get_info(card, 0x02, 0xA4, info, sizeof(info)) >= 0) {
		if (info[12] & 0x40) {
			priv->offset_shift = 0;
			priv->offset_mask = 0;
		}
		if (info[12] & 0x10) {
			 
		}
		if (info[12] & 0x08) {
			priv->locked = 1;
		}
		 
		if (!memcmp(info+5, "\x00\xff\x00", 3)) {
			priv->sample_card = 1;
		}
	}

	 
	card->caps |= SC_CARD_CAP_RNG;

	 
	card->max_recv_size = 252;

	return SC_SUCCESS;
}