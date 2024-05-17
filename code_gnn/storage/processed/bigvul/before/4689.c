zend_bool php_openssl_pkey_init_dsa(DSA *dsa)
{
	if (!dsa->p || !dsa->q || !dsa->g) {
		return 0;
	}
	if (dsa->priv_key || dsa->pub_key) {
		return 1;
	}
	PHP_OPENSSL_RAND_ADD_TIME();
	if (!DSA_generate_key(dsa)) {
		return 0;
	}
	 
	if (!dsa->pub_key || BN_is_zero(dsa->pub_key)) {
		return 0;
	}
	 
	return 1;
}
