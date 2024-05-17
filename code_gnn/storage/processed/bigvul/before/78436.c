gpk_compute_crycks(sc_card_t *card, sc_apdu_t *apdu,
			u8 *crycks1)
{
	struct gpk_private_data *priv = DRVDATA(card);
	u8		in[8], out[8], block[64];
	unsigned int	len = 0, i;
	int             r = SC_SUCCESS, outl;
	EVP_CIPHER_CTX  *ctx = NULL;

	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL)
		return SC_ERROR_INTERNAL;


	 
	memset(block, 0x00, sizeof(block));
	block[len++] = apdu->cla;
	block[len++] = apdu->ins;
	block[len++] = apdu->p1;
	block[len++] = apdu->p2;
	block[len++] = apdu->lc + 3;
	if ((i = apdu->datalen) + len > sizeof(block))
		i = sizeof(block) - len;
	memcpy(block+len, apdu->data, i);
	len += i;

	 
	memset(in, 0x00, 8);

	EVP_EncryptInit_ex(ctx, EVP_des_ede_cbc(), NULL, priv->key, in);
	for (i = 0; i < len; i += 8) {
		if (!EVP_EncryptUpdate(ctx, out, &outl, &block[i], 8)) {
			r = SC_ERROR_INTERNAL;
			break;
		}
	}
	EVP_CIPHER_CTX_free(ctx);

	memcpy((u8 *) (apdu->data + apdu->datalen), out + 5, 3);
	apdu->datalen += 3;
	apdu->lc += 3;
	apdu->le += 3;
	if (crycks1)
		memcpy(crycks1, out, 3);
	sc_mem_clear(in, sizeof(in));
	sc_mem_clear(out, sizeof(out));
	sc_mem_clear(block, sizeof(block));
	return r;
}