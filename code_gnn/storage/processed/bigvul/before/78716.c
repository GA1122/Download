static int itacns_check_and_add_keyset(sc_pkcs15_card_t *p15card,
	const char *label, int sec_env, size_t cert_offset,
	const char *cert_path, const char *pubkey_path, const char *prkey_path,
	u8 pin_ref, int *found_certificates)
{
	int r;
	sc_path_t path;
	sc_pkcs15_id_t cert_id;
	int ext_info_ok;
	int ku = 0, xku = 0;
	int pubkey_usage_flags = 0, prkey_usage_flags = 0;

	cert_id.len = 1;
	cert_id.value[0] = sec_env;
	*found_certificates = 0;

	 
	if (!cert_path) {
		sc_debug(p15card->card->ctx, SC_LOG_DEBUG_NORMAL,
			"We cannot use keys without a matching certificate");
		return SC_ERROR_NOT_SUPPORTED;
	}

	sc_format_path(cert_path, &path);
	r = sc_select_file(p15card->card, &path, NULL);
	if (r == SC_ERROR_FILE_NOT_FOUND)
		return 0;
	if (r != SC_SUCCESS) {
		sc_debug(p15card->card->ctx, SC_LOG_DEBUG_NORMAL,
			"Could not find certificate for %s", label);
		return r;
	}

	 
	if (cert_offset) {
		u8 certlen[3];
		r = loadFile(p15card, &path, certlen, sizeof(certlen));
		SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
			"Could not read certificate file");
		path.index = cert_offset;
		path.count = (certlen[1] << 8) + certlen[2];
		 
		if (path.count == 0)
			return 0;
	}

	r = itacns_add_cert(p15card, SC_PKCS15_TYPE_CERT_X509, 0,
		&path, &cert_id, label, 0, &ext_info_ok, &ku, &xku);
	if (r == SC_ERROR_INVALID_ASN1_OBJECT)
		return 0;
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not add certificate");
	(*found_certificates)++;

	 
	if(ext_info_ok) {
#ifdef ENABLE_OPENSSL
		if (ku & KU_DIGITAL_SIGNATURE) {
			pubkey_usage_flags |= SC_PKCS15_PRKEY_USAGE_VERIFY;
			prkey_usage_flags |= SC_PKCS15_PRKEY_USAGE_SIGN;
		}
		if (ku & KU_NON_REPUDIATION) {
			pubkey_usage_flags |= SC_PKCS15_PRKEY_USAGE_VERIFY;
			prkey_usage_flags |= SC_PKCS15_PRKEY_USAGE_NONREPUDIATION;
		}
		if (ku & KU_KEY_ENCIPHERMENT || ku & KU_KEY_AGREEMENT
			|| xku & XKU_SSL_CLIENT) {
			pubkey_usage_flags |= SC_PKCS15_PRKEY_USAGE_WRAP;
			prkey_usage_flags |= SC_PKCS15_PRKEY_USAGE_UNWRAP;
		}
		if (ku & KU_DATA_ENCIPHERMENT || xku & XKU_SMIME) {
			pubkey_usage_flags |= SC_PKCS15_PRKEY_USAGE_ENCRYPT;
			prkey_usage_flags |= SC_PKCS15_PRKEY_USAGE_DECRYPT;
		}
#else  
		sc_debug(p15card->card->ctx, SC_LOG_DEBUG_NORMAL,
			"Extended certificate info retrieved without OpenSSL. "
			"How is this possible?");
		return SC_ERROR_INTERNAL;
#endif  
	} else {
		 
		pubkey_usage_flags =
			  SC_PKCS15_PRKEY_USAGE_VERIFY
			| SC_PKCS15_PRKEY_USAGE_WRAP;
		prkey_usage_flags =
			  SC_PKCS15_PRKEY_USAGE_SIGN
			| SC_PKCS15_PRKEY_USAGE_UNWRAP;
	}

	r = itacns_add_keyset(p15card, label, sec_env, &cert_id,
		pubkey_path, prkey_path, pubkey_usage_flags, prkey_usage_flags,
		pin_ref);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not add keys for this certificate");

	return r;
}