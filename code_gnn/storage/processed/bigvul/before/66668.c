static int crypto_rfc4309_encrypt(struct aead_request *req)
{
	if (req->assoclen != 16 && req->assoclen != 20)
		return -EINVAL;

	req = crypto_rfc4309_crypt(req);

	return crypto_aead_encrypt(req);
}