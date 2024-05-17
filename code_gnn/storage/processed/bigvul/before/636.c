static bool decode_dirsync_request(void *mem_ctx, DATA_BLOB in, void *_out)
{
	void **out = (void **)_out;
	DATA_BLOB cookie;
	struct asn1_data *data = asn1_init(mem_ctx);
	struct ldb_dirsync_control *ldc;

	if (!data) return false;

	if (!asn1_load(data, in)) {
		return false;
	}

	ldc = talloc(mem_ctx, struct ldb_dirsync_control);
	if (!ldc) {
		return false;
	}

	if (!asn1_start_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_read_Integer(data, &(ldc->flags))) {
		return false;
	}
	
	if (!asn1_read_Integer(data, &(ldc->max_attributes))) {
		return false;
	}
	
	if (!asn1_read_OctetString(data, mem_ctx, &cookie)) {
		return false;
	}
	ldc->cookie_len = cookie.length;
	if (ldc->cookie_len) {
		ldc->cookie = talloc_memdup(ldc, cookie.data, cookie.length);

		if (!(ldc->cookie)) {
			return false;
		}
	} else {
		ldc->cookie = NULL;
	}

	if (!asn1_end_tag(data)) {
		return false;
	}

	*out = ldc;

	return true;
}
