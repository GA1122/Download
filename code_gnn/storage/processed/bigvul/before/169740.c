exsltCryptoGcryptHash (xmlXPathParserContextPtr ctxt ATTRIBUTE_UNUSED,
 
		       int algorithm, const char *msg,
		       unsigned long msglen,
		       char dest[HASH_DIGEST_LENGTH]) {
    exsltCryptoGcryptInit ();
    gcry_md_hash_buffer (algorithm, dest, msg, msglen);
}
