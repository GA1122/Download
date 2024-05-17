static int __init big_key_init(void)
{
	int ret;

	 
	big_key_aead = crypto_alloc_aead(big_key_alg_name, 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(big_key_aead)) {
		ret = PTR_ERR(big_key_aead);
		pr_err("Can't alloc crypto: %d\n", ret);
		return ret;
	}
	ret = crypto_aead_setauthsize(big_key_aead, ENC_AUTHTAG_SIZE);
	if (ret < 0) {
		pr_err("Can't set crypto auth tag len: %d\n", ret);
		goto free_aead;
	}

	ret = register_key_type(&key_type_big_key);
	if (ret < 0) {
		pr_err("Can't register type: %d\n", ret);
		goto free_aead;
	}

	return 0;

free_aead:
	crypto_free_aead(big_key_aead);
	return ret;
}