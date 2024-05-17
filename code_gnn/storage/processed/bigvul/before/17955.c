kex_free(struct kex *kex)
{
	u_int mode;

#ifdef WITH_OPENSSL
	if (kex->dh)
		DH_free(kex->dh);
#ifdef OPENSSL_HAS_ECC
	if (kex->ec_client_key)
		EC_KEY_free(kex->ec_client_key);
#endif  
#endif  
	for (mode = 0; mode < MODE_MAX; mode++) {
		kex_free_newkeys(kex->newkeys[mode]);
		kex->newkeys[mode] = NULL;
	}
	sshbuf_free(kex->peer);
	sshbuf_free(kex->my);
	free(kex->session_id);
	free(kex->client_version_string);
	free(kex->server_version_string);
	free(kex->failed_choice);
	free(kex->hostkey_alg);
	free(kex->name);
	free(kex);
}