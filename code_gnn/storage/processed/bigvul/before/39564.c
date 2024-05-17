BOOL license_read_license_request_packet(rdpLicense* license, wStream* s)
{
	 
	if (Stream_GetRemainingLength(s) < 32)
		return FALSE;

	Stream_Read(s, license->ServerRandom, 32);

	 
	if (!license_read_product_info(s, license->ProductInfo))
		return FALSE;

	 
	if (!license_read_binary_blob(s, license->KeyExchangeList))
		return FALSE;

	 
	if (!license_read_binary_blob(s, license->ServerCertificate))
		return FALSE;

	 
	if (!license_read_scope_list(s, license->ScopeList))
		return FALSE;

	 
	if (!certificate_read_server_certificate(license->certificate,
			license->ServerCertificate->data, license->ServerCertificate->length) < 0)
		return FALSE;

	license_generate_keys(license);
	license_generate_hwid(license);
	license_encrypt_premaster_secret(license);

#ifdef WITH_DEBUG_LICENSE
	fprintf(stderr, "ServerRandom:\n");
	winpr_HexDump(license->ServerRandom, 32);
	fprintf(stderr, "\n");

	license_print_product_info(license->ProductInfo);
	fprintf(stderr, "\n");

	license_print_scope_list(license->ScopeList);
	fprintf(stderr, "\n");
#endif

	return TRUE;
}
