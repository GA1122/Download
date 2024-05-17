coolkey_logout(sc_card_t *card)
{
	 
	coolkey_private_data_t * priv = COOLKEY_DATA(card);
	u8 pin_ref = 0;

	(void) coolkey_apdu_io(card, COOLKEY_CLASS, COOLKEY_INS_LOGOUT, pin_ref, 0, NULL, 0, NULL, NULL,
		priv->nonce, sizeof(priv->nonce));
	 
	memset(priv->nonce, 0, sizeof(priv->nonce));
	priv->nonce_valid = 0;
	return SC_SUCCESS;
}
