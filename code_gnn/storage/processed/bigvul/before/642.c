static bool decode_server_sort_request(void *mem_ctx, DATA_BLOB in, void *_out)
{
	void **out = (void **)_out;
	DATA_BLOB attr;
	DATA_BLOB rule;
	struct asn1_data *data = asn1_init(mem_ctx);
	struct ldb_server_sort_control **lssc;
	int num;

	if (!data) return false;

	if (!asn1_load(data, in)) {
		return false;
	}

	if (!asn1_start_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	lssc = NULL;

	for (num = 0; asn1_peek_tag(data, ASN1_SEQUENCE(0)); num++) {
		lssc = talloc_realloc(mem_ctx, lssc, struct ldb_server_sort_control *, num + 2);
		if (!lssc) {
			return false;
		}
		lssc[num] = talloc_zero(lssc, struct ldb_server_sort_control);
		if (!lssc[num]) {
			return false;
		}

		if (!asn1_start_tag(data, ASN1_SEQUENCE(0))) {
			return false;
		}

		if (!asn1_read_OctetString(data, mem_ctx, &attr)) {
			return false;
		}

		lssc[num]->attributeName = talloc_strndup(lssc[num], (const char *)attr.data, attr.length);
		if (!lssc [num]->attributeName) {
			return false;
		}
	
		if (asn1_peek_tag(data, ASN1_OCTET_STRING)) {
			if (!asn1_read_OctetString(data, mem_ctx, &rule)) {
				return false;
			}
			lssc[num]->orderingRule = talloc_strndup(lssc[num], (const char *)rule.data, rule.length);
			if (!lssc[num]->orderingRule) {
				return false;
			}
		}

		if (asn1_peek_tag(data, ASN1_CONTEXT_SIMPLE(1))) {
			bool reverse;
			if (!asn1_read_BOOLEAN_context(data, &reverse, 1)) {
			return false;
			}
			lssc[num]->reverse = reverse;
		}
	
		if (!asn1_end_tag(data)) {
			return false;
		}
	}

	if (lssc != NULL) {
		lssc[num] = NULL;
	}

	if (!asn1_end_tag(data)) {
		return false;
	}

	*out = lssc;

	return true;
}
