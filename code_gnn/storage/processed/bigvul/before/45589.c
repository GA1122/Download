static int crypto_rfc4309_decrypt(struct aead_request *req)
{
	req = crypto_rfc4309_crypt(req);

	return crypto_aead_decrypt(req);
}