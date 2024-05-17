exsltCryptoRegister (void) {
    xsltRegisterExtModuleFunction ((const xmlChar *) "md4",
				   EXSLT_CRYPTO_NAMESPACE,
				   exsltCryptoMd4Function);
    xsltRegisterExtModuleFunction ((const xmlChar *) "md5",
				   EXSLT_CRYPTO_NAMESPACE,
				   exsltCryptoMd5Function);
    xsltRegisterExtModuleFunction ((const xmlChar *) "sha1",
				   EXSLT_CRYPTO_NAMESPACE,
				   exsltCryptoSha1Function);
    xsltRegisterExtModuleFunction ((const xmlChar *) "rc4_encrypt",
				   EXSLT_CRYPTO_NAMESPACE,
				   exsltCryptoRc4EncryptFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "rc4_decrypt",
				   EXSLT_CRYPTO_NAMESPACE,
				   exsltCryptoRc4DecryptFunction);
}
