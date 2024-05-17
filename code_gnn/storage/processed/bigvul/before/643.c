static bool decode_server_sort_response(void *mem_ctx, DATA_BLOB in, void *_out)
{
	void **out = (void **)_out;
	DATA_BLOB attr;
	struct asn1_data *data = asn1_init(mem_ctx);
	struct ldb_sort_resp_control *lsrc;

	if (!data) return false;

	if (!asn1_load(data, in)) {
		return false;
	}

	lsrc = talloc(mem_ctx, struct ldb_sort_resp_control);
	if (!lsrc) {
		return false;
	}

	if (!asn1_start_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_read_enumerated(data, &(lsrc->result))) {
		return false;
	}

	lsrc->attr_desc = NULL;
	if (asn1_peek_tag(data, ASN1_OCTET_STRING)) {
		if (!asn1_read_OctetString(data, mem_ctx, &attr)) {
			return false;
		}
		lsrc->attr_desc = talloc_strndup(lsrc, (const char *)attr.data, attr.length);
		if (!lsrc->attr_desc) {
			return false;
		}
	}

	if (!asn1_end_tag(data)) {
		return false;
	}

	*out = lsrc;

	return true;
}
