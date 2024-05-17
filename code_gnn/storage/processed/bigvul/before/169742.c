exsltCryptoGcryptRc4Decrypt (xmlXPathParserContextPtr ctxt,
			     const unsigned char *key,
			     const unsigned char *msg, int msglen,
			     unsigned char *dest, int destlen) {
    gcry_cipher_hd_t cipher;
    gcry_error_t rc = 0;

    exsltCryptoGcryptInit ();

    rc = gcry_cipher_open (&cipher, GCRY_CIPHER_ARCFOUR,
			   GCRY_CIPHER_MODE_STREAM, 0);
    if (rc) {
	xsltTransformError (xsltXPathGetTransformContext (ctxt), NULL,
			    NULL,
			    "exslt:crypto internal error %s (gcry_cipher_open)\n",
			    gcry_strerror (rc));
    }

    rc = gcry_cipher_setkey (cipher, key, RC4_KEY_LENGTH);
    if (rc) {
	xsltTransformError (xsltXPathGetTransformContext (ctxt), NULL,
			    NULL,
			    "exslt:crypto internal error %s (gcry_cipher_setkey)\n",
			    gcry_strerror (rc));
    }

    rc = gcry_cipher_decrypt (cipher, (unsigned char *) dest, destlen,
			      (const unsigned char *) msg, msglen);
    if (rc) {
	xsltTransformError (xsltXPathGetTransformContext (ctxt), NULL,
			    NULL,
			    "exslt:crypto internal error %s (gcry_cipher_decrypt)\n",
			    gcry_strerror (rc));
    }

    gcry_cipher_close (cipher);
}
