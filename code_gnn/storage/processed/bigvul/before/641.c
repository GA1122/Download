static bool decode_search_options_request(void *mem_ctx, DATA_BLOB in, void *_out)
{
	void **out = (void **)_out;
	struct asn1_data *data = asn1_init(mem_ctx);
	struct ldb_search_options_control *lsoc;

	if (!data) return false;

	if (!asn1_load(data, in)) {
		return false;
	}

	lsoc = talloc(mem_ctx, struct ldb_search_options_control);
	if (!lsoc) {
		return false;
	}

	if (!asn1_start_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_read_Integer(data, (int *) &(lsoc->search_options))) {
		return false;
	}

	if (!asn1_end_tag(data)) {
		return false;
	}

	*out = lsoc;

	return true;
}
