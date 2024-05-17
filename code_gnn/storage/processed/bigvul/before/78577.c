pgp_calculate_and_store_fingerprint(sc_card_t *card, time_t ctime,
                                    u8* modulus, u8* exponent,
                                    sc_cardctl_openpgp_keygen_info_t *key_info)
{
	u8 fingerprint[SHA_DIGEST_LENGTH];
	size_t mlen = key_info->modulus_len >> 3;   
	size_t elen = key_info->exponent_len >> 3;   
	u8 *fp_buffer = NULL;   
	size_t fp_buffer_len;
	u8 *p;  
	size_t pk_packet_len;
	unsigned int tag;
	pgp_blob_t *fpseq_blob;
	u8 *newdata;
	int r;

	LOG_FUNC_CALLED(card->ctx);

	if (modulus == NULL || exponent == NULL || mlen == 0 || elen == 0) {
		sc_log(card->ctx, "Null data (modulus or exponent)");
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_INVALID_ARGUMENTS);
	}

	 
	pk_packet_len =   1    
	                + 4    
	                + 1    
	                + 2    
	                + mlen
	                + 2
	                + elen;

	fp_buffer_len = 3 + pk_packet_len;
	p = fp_buffer = calloc(fp_buffer_len, 1);
	if (!p) {
		LOG_FUNC_RETURN(card->ctx, SC_ERROR_NOT_ENOUGH_MEMORY);
	}

	p[0] = 0x99;    
	ushort2bebytes(++p, (unsigned short)pk_packet_len);
	 
	p += 2;
	*p = 4;         
	ulong2bebytes(++p, (unsigned long)ctime);     
	p += 4;
	*p = 1;         
	 
	ushort2bebytes(++p, (unsigned short)key_info->modulus_len);
	p += 2;
	memcpy(p, modulus, mlen);
	p += mlen;
	ushort2bebytes(++p, (unsigned short)key_info->exponent_len);
	p += 2;
	memcpy(p, exponent, elen);
	p = NULL;

	 
	SHA1(fp_buffer, fp_buffer_len, fingerprint);
	free(fp_buffer);

	 
	tag = 0x00C6 + key_info->keytype;
	sc_log(card->ctx, "Write to DO %04X.", tag);
	r = pgp_put_data(card, 0x00C6 + key_info->keytype, fingerprint, SHA_DIGEST_LENGTH);
	LOG_TEST_RET(card->ctx, r, "Cannot write to DO.");

	 
	sc_log(card->ctx, "Update the blob containing fingerprints (00C5)");
	fpseq_blob = pgp_find_blob(card, 0x00C5);
	if (!fpseq_blob) {
		sc_log(card->ctx, "Not found 00C5");
		goto exit;
	}
	 
	newdata = malloc(fpseq_blob->len);
	if (!newdata) {
		sc_log(card->ctx, "Not enough memory to update fingerprints blob.");
		goto exit;
	}
	memcpy(newdata, fpseq_blob->data, fpseq_blob->len);
	 
	p = newdata + 20*(key_info->keytype - 1);
	 
	memcpy(p, fingerprint, 20);
	 
	pgp_set_blob(fpseq_blob, newdata, fpseq_blob->len);
	free(newdata);

exit:
	LOG_FUNC_RETURN(card->ctx, r);
}
