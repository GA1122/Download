static int itacns_add_cert(sc_pkcs15_card_t *p15card,
	int type, int authority, const sc_path_t *path,
	const sc_pkcs15_id_t *id, const char *label, int obj_flags,
	int *ext_info_ok, int *key_usage, int *x_key_usage)
{
	int r;
	 
	sc_pkcs15_cert_info_t info;
	sc_pkcs15_object_t    obj;
#ifdef ENABLE_OPENSSL
	X509 *x509;
	sc_pkcs15_cert_t *cert;
#endif

	SC_FUNC_CALLED(p15card->card->ctx, 1);
	
	if(type != SC_PKCS15_TYPE_CERT_X509) {
		sc_debug(p15card->card->ctx, SC_LOG_DEBUG_NORMAL,
			"Cannot add a certificate of a type other than X.509");
		return 1;
	}
	
	*ext_info_ok = 0;
	

	memset(&info, 0, sizeof(info));
	memset(&obj,  0, sizeof(obj));

	info.id                = *id;
	info.authority         = authority;
	if (path)
		info.path = *path;

	strlcpy(obj.label, label, sizeof(obj.label));
	obj.flags = obj_flags;

	r = sc_pkcs15emu_add_x509_cert(p15card, &obj, &info);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not add X.509 certificate");

	 
#ifdef ENABLE_OPENSSL

	r = sc_pkcs15_read_certificate(p15card, &info, &cert);
	SC_TEST_RET(p15card->card->ctx, SC_LOG_DEBUG_NORMAL, r,
		"Could not read X.509 certificate");

	{
		const u8 *throwaway = cert->data.value;
		x509 = d2i_X509(NULL, &throwaway, cert->data.len);
	}
	sc_pkcs15_free_certificate(cert);
	if (!x509) return SC_SUCCESS;
	X509_check_purpose(x509, -1, 0);

	if(X509_get_extension_flags(x509) & EXFLAG_KUSAGE) {
		*ext_info_ok = 1;
		*key_usage = X509_get_key_usage(x509);
		*x_key_usage = X509_get_extended_key_usage(x509);
	}
	OPENSSL_free(x509);

	return SC_SUCCESS;

#else  

	return SC_SUCCESS;

#endif  

}