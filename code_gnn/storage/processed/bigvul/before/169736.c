exsltCryptoCryptoApiHash (xmlXPathParserContextPtr ctxt,
			  ALG_ID algorithm, const char *msg,
			  unsigned long msglen,
			  char dest[HASH_DIGEST_LENGTH]) {
    HCRYPTPROV hCryptProv;
    HCRYPTHASH hHash;

    if (!CryptAcquireContext (&hCryptProv, NULL, NULL, PROV_RSA_FULL,
			      CRYPT_VERIFYCONTEXT | CRYPT_SILENT)) {
	exsltCryptoCryptoApiReportError (ctxt, __LINE__);
	return;
    }

    hHash = exsltCryptoCryptoApiCreateHash (ctxt, hCryptProv,
					    algorithm, msg, msglen,
					    dest, HASH_DIGEST_LENGTH);
    if (0 != hHash) {
	CryptDestroyHash (hHash);
    }

    CryptReleaseContext (hCryptProv, 0);
}
