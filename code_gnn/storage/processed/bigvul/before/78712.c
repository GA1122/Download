static int itacns_add_keyset(sc_pkcs15_card_t *p15card,
	const char *label, int sec_env, sc_pkcs15_id_t *cert_id,
	const char *pubkey_path, const char *prkey_path,
	unsigned int pubkey_usage_flags, unsigned int prkey_usage_flags,
	u8 pin_ref)
{
	int r;
	sc_path_t path;
	sc_path_t *private_path = NULL;
	char pinlabel[16];
	int fake_puk_authid, pin_flags;

	 
	int modulus_length = 1024;

	 
	 
	if (pubkey_path) {
		sc_format_path(pubkey_path, &path);
		r = itacns_add_pubkey(p15card, &path, cert_id, label,
			pubkey_usage_flags, sec_env, 0, &modulus_length);
		SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
			"Could not add public key");
	}

	 
	if (prkey_path) {
		sc_format_path(prkey_path, &path);
		private_path = &path;
	}
	r = itacns_add_prkey(p15card, cert_id, label, SC_PKCS15_TYPE_PRKEY_RSA,
		modulus_length,
		prkey_usage_flags,
		private_path, sec_env, cert_id, SC_PKCS15_CO_FLAG_PRIVATE);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not add private key");

	 
	strlcpy(pinlabel, "PIN ", sizeof(pinlabel));
	strlcat(pinlabel, label, sizeof(pinlabel));

	 
	fake_puk_authid = 0x90 + pin_ref;
	pin_flags = SC_PKCS15_PIN_FLAG_CASE_SENSITIVE
		| SC_PKCS15_PIN_FLAG_INITIALIZED;
	r = itacns_add_pin(p15card, pinlabel, sec_env, fake_puk_authid, pin_ref,
	    private_path, pin_flags);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not add PIN");

	strlcpy(pinlabel, "PUK ", sizeof(pinlabel));
	strlcat(pinlabel, label, sizeof(pinlabel));
	 
	pin_flags |= SC_PKCS15_PIN_FLAG_UNBLOCKING_PIN
	| SC_PKCS15_PIN_FLAG_UNBLOCK_DISABLED;
	r = itacns_add_pin(p15card, pinlabel, fake_puk_authid, 0, pin_ref+1,
	    private_path, pin_flags);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not add PUK");

	return 0;
}
