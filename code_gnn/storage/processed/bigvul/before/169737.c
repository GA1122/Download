exsltCryptoCryptoApiRc4Decrypt (xmlXPathParserContextPtr ctxt,
				const unsigned char *key,
				const unsigned char *msg, int msglen,
				unsigned char *dest, int destlen) {
    HCRYPTPROV hCryptProv;
    HCRYPTKEY hKey;
    HCRYPTHASH hHash;
    DWORD dwDataLen;
    unsigned char hash[HASH_DIGEST_LENGTH];

    if (msglen > destlen) {
	xsltTransformError (xsltXPathGetTransformContext (ctxt), NULL,
			    NULL,
			    "exslt:crypto : internal error exsltCryptoCryptoApiRc4Encrypt dest buffer too small.\n");
	return;
    }

    if (!CryptAcquireContext (&hCryptProv, NULL, NULL, PROV_RSA_FULL,
			      CRYPT_VERIFYCONTEXT | CRYPT_SILENT)) {
	exsltCryptoCryptoApiReportError (ctxt, __LINE__);
	return;
    }

    hHash = exsltCryptoCryptoApiCreateHash (ctxt, hCryptProv,
					    CALG_SHA1, key,
					    RC4_KEY_LENGTH, hash,
					    HASH_DIGEST_LENGTH);

    if (!CryptDeriveKey
	(hCryptProv, CALG_RC4, hHash, 0x00800000, &hKey)) {
	exsltCryptoCryptoApiReportError (ctxt, __LINE__);
	goto fail;
    }
 
    dwDataLen = msglen;
    memcpy (dest, msg, msglen);
    if (!CryptDecrypt (hKey, 0, TRUE, 0, dest, &dwDataLen)) {
	exsltCryptoCryptoApiReportError (ctxt, __LINE__);
	goto fail;
    }

  fail:
    if (0 != hHash) {
	CryptDestroyHash (hHash);
    }

    CryptDestroyKey (hKey);
    CryptReleaseContext (hCryptProv, 0);
}
