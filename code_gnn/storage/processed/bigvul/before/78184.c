authentic_fcp_encode(struct sc_card *card, struct sc_file *file, unsigned char *out, size_t out_len)
{
	struct sc_context *ctx = card->ctx;
	unsigned char buf[0x80];
	size_t ii, offs;
	unsigned char ops_ef[4] = { SC_AC_OP_READ, SC_AC_OP_DELETE, SC_AC_OP_UPDATE, SC_AC_OP_RESIZE };
	unsigned char ops_df[3] = { SC_AC_OP_CREATE, SC_AC_OP_DELETE, SC_AC_OP_CRYPTO };
	unsigned char *ops = file->type == SC_FILE_TYPE_DF ? ops_df : ops_ef;
	size_t ops_len = file->type == SC_FILE_TYPE_DF ? 3 : 4;

	LOG_FUNC_CALLED(ctx);

	offs = 0;
	buf[offs++] = ISO7816_TAG_FCP_SIZE;
	buf[offs++] = 2;
	buf[offs++] = (file->size >> 8) & 0xFF;
	buf[offs++] = file->size & 0xFF;

	buf[offs++] = ISO7816_TAG_FCP_TYPE;
	buf[offs++] = 1;
	buf[offs++] = file->type == SC_FILE_TYPE_DF ? ISO7816_FILE_TYPE_DF : ISO7816_FILE_TYPE_TRANSPARENT_EF;

	buf[offs++] = ISO7816_TAG_FCP_FID;
	buf[offs++] = 2;
	buf[offs++] = (file->id >> 8) & 0xFF;
	buf[offs++] = file->id & 0xFF;

	buf[offs++] = ISO7816_TAG_FCP_ACLS;
	buf[offs++] = ops_len * 2;
	for (ii=0; ii < ops_len; ii++) {
		const struct sc_acl_entry *entry;

		entry = sc_file_get_acl_entry(file, ops[ii]);
		sc_log(ctx, "acl entry(method:%X,ref:%X)", entry->method, entry->key_ref);

		if (entry->method == SC_AC_NEVER)   {
			 
			buf[offs++] = 0x00;
			buf[offs++] = 0x00;
		}
		else if (entry->method == SC_AC_NONE)   {
			buf[offs++] = 0x00;
			buf[offs++] = 0x00;
		}
		else if (entry->method == SC_AC_CHV)   {
			if (!(entry->key_ref & AUTHENTIC_V3_CREDENTIAL_ID_MASK)
					|| (entry->key_ref & ~AUTHENTIC_V3_CREDENTIAL_ID_MASK))
				LOG_TEST_RET(ctx, SC_ERROR_NOT_SUPPORTED, "Non supported Credential Reference");
			buf[offs++] = 0x00;
			buf[offs++] = 0x01 << (entry->key_ref - 1);
		}
		else
			LOG_TEST_RET(ctx, SC_ERROR_NOT_SUPPORTED, "Non supported AC method");
	}

	if (out)   {
		if (out_len < offs)
			LOG_TEST_RET(ctx, SC_ERROR_BUFFER_TOO_SMALL, "Buffer too small to encode FCP");
		memcpy(out, buf, offs);
	}

	LOG_FUNC_RETURN(ctx, offs);
}