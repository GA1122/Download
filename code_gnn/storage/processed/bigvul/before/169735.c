exsltCryptoCryptoApiCreateHash (xmlXPathParserContextPtr ctxt,
				HCRYPTPROV hCryptProv, ALG_ID algorithm,
				const char *msg, unsigned int msglen,
				char *dest, unsigned int destlen)
{
    HCRYPTHASH hHash = 0;
    DWORD dwHashLen = destlen;

    if (!CryptCreateHash (hCryptProv, algorithm, 0, 0, &hHash)) {
	exsltCryptoCryptoApiReportError (ctxt, __LINE__);
	return 0;
    }

    if (!CryptHashData (hHash, (const BYTE *) msg, msglen, 0)) {
	exsltCryptoCryptoApiReportError (ctxt, __LINE__);
	goto fail;
    }

    if (!CryptGetHashParam (hHash, HP_HASHVAL, dest, &dwHashLen, 0)) {
	exsltCryptoCryptoApiReportError (ctxt, __LINE__);
	goto fail;
    }

  fail:
    return hHash;
}
