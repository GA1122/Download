bool ber_write_partial_OID_String(TALLOC_CTX *mem_ctx, DATA_BLOB *blob, const char *partial_oid)
{
	TALLOC_CTX *tmp_ctx = talloc_new(mem_ctx);
	char *oid = talloc_strdup(tmp_ctx, partial_oid);
	char *p;

	 
	p = strchr(oid, ':');
	if (p) {
		*p = '\0';
		p++;
	}

	if (!ber_write_OID_String(mem_ctx, blob, oid)) {
		talloc_free(tmp_ctx);
		return false;
	}

	 
	if (p) {
		DATA_BLOB tmp_blob = strhex_to_data_blob(tmp_ctx, p);
		if (!data_blob_append(mem_ctx, blob, tmp_blob.data,
				      tmp_blob.length)) {
			talloc_free(tmp_ctx);
			return false;
		}
	}

	talloc_free(tmp_ctx);

	return true;
}
