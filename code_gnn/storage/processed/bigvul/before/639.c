static bool decode_paged_results_request(void *mem_ctx, DATA_BLOB in, void *_out)
{
	void **out = (void **)_out;
	DATA_BLOB cookie;
	struct asn1_data *data = asn1_init(mem_ctx);
	struct ldb_paged_control *lprc;

	if (!data) return false;

	if (!asn1_load(data, in)) {
		return false;
	}

	lprc = talloc(mem_ctx, struct ldb_paged_control);
	if (!lprc) {
		return false;
	}

	if (!asn1_start_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_read_Integer(data, &(lprc->size))) {
		return false;
	}
	
	if (!asn1_read_OctetString(data, mem_ctx, &cookie)) {
		return false;
	}
	lprc->cookie_len = cookie.length;
	if (lprc->cookie_len) {
		lprc->cookie = talloc_memdup(lprc, cookie.data, cookie.length);

		if (!(lprc->cookie)) {
			return false;
		}
	} else {
		lprc->cookie = NULL;
	}

	if (!asn1_end_tag(data)) {
		return false;
	}

	*out = lprc;

	return true;
}
