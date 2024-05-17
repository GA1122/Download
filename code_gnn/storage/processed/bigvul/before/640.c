static bool decode_sd_flags_request(void *mem_ctx, DATA_BLOB in, void *_out)
{
	void **out = (void **)_out;
	struct asn1_data *data = asn1_init(mem_ctx);
	struct ldb_sd_flags_control *lsdfc;

	if (!data) return false;

	if (!asn1_load(data, in)) {
		return false;
	}

	lsdfc = talloc(mem_ctx, struct ldb_sd_flags_control);
	if (!lsdfc) {
		return false;
	}

	if (!asn1_start_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_read_Integer(data, (int *) &(lsdfc->secinfo_flags))) {
		return false;
	}

	if (!asn1_end_tag(data)) {
		return false;
	}

	*out = lsdfc;

	return true;
}
