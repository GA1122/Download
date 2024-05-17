 static int key_verify(pam_handle_t *pamh, int flags, PKCS11_KEY *authkey)
  {
  	int ok = 0;
  	unsigned char challenge[30];
	unsigned char signature[256];
	unsigned int siglen = sizeof signature;
// 	unsigned char *signature = NULL;
// 	unsigned int siglen;
  	const EVP_MD *md = EVP_sha1();
  	EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
  	EVP_PKEY *privkey = PKCS11_get_private_key(authkey);
  	EVP_PKEY *pubkey = PKCS11_get_public_key(authkey);
  
// 	if (NULL == privkey)
// 		goto err;
// 	siglen = EVP_PKEY_size(privkey);
// 	if (siglen <= 0)
// 		goto err;
// 	signature = malloc(siglen);
// 	if (NULL == signature)
// 		goto err;
// 
  	 
 	if (1 != randomize(pamh, challenge, sizeof challenge)) {
 		goto err;
 	}
 	if (NULL == pubkey || NULL == privkey || NULL == md_ctx || NULL == md
 			|| !EVP_SignInit(md_ctx, md)
 			|| !EVP_SignUpdate(md_ctx, challenge, sizeof challenge)
 			|| !EVP_SignFinal(md_ctx, signature, &siglen, privkey)
 			|| !EVP_MD_CTX_reset(md_ctx)
 			|| !EVP_VerifyInit(md_ctx, md)
 			|| !EVP_VerifyUpdate(md_ctx, challenge, sizeof challenge)
 			|| 1 != EVP_VerifyFinal(md_ctx, signature, siglen, pubkey)) {
 		pam_syslog(pamh, LOG_DEBUG, "Error verifying key: %s\n",
 				ERR_reason_error_string(ERR_get_error()));
 		prompt(flags, pamh, PAM_ERROR_MSG, NULL, _("Error verifying key"));
 		goto err;
 	}
  	ok = 1;
  
  err:
// 	free(signature);
  	if (NULL != pubkey)
  		EVP_PKEY_free(pubkey);
  	if (NULL != privkey)
 		EVP_PKEY_free(privkey);
 	if (NULL != md_ctx) {
 		EVP_MD_CTX_free(md_ctx);
 	}
 	return ok;
 }