static int big_key_crypt(enum big_key_op op, u8 *data, size_t datalen, u8 *key)
{
	int ret;
	struct scatterlist sgio;
	struct aead_request *aead_req;
	 
	u8 zero_nonce[crypto_aead_ivsize(big_key_aead)];

	aead_req = aead_request_alloc(big_key_aead, GFP_KERNEL);
	if (!aead_req)
		return -ENOMEM;

	memset(zero_nonce, 0, sizeof(zero_nonce));
	sg_init_one(&sgio, data, datalen + (op == BIG_KEY_ENC ? ENC_AUTHTAG_SIZE : 0));
	aead_request_set_crypt(aead_req, &sgio, &sgio, datalen, zero_nonce);
	aead_request_set_callback(aead_req, CRYPTO_TFM_REQ_MAY_SLEEP, NULL, NULL);
	aead_request_set_ad(aead_req, 0);

	mutex_lock(&big_key_aead_lock);
	if (crypto_aead_setkey(big_key_aead, key, ENC_KEY_SIZE)) {
		ret = -EAGAIN;
		goto error;
	}
	if (op == BIG_KEY_ENC)
		ret = crypto_aead_encrypt(aead_req);
	else
		ret = crypto_aead_decrypt(aead_req);
error:
	mutex_unlock(&big_key_aead_lock);
	aead_request_free(aead_req);
	return ret;
}
