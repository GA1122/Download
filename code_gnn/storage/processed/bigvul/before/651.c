static bool encode_sd_flags_request(void *mem_ctx, void *in, DATA_BLOB *out)
{
	struct ldb_sd_flags_control *lsdfc = talloc_get_type(in, struct ldb_sd_flags_control);
	struct asn1_data *data = asn1_init(mem_ctx);

	if (!data) return false;

	if (!asn1_push_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_write_Integer(data, lsdfc->secinfo_flags)) {
		return false;
	}

	if (!asn1_pop_tag(data)) {
		return false;
	}

	*out = data_blob_talloc(mem_ctx, data->data, data->length);
	if (out->data == NULL) {
		return false;
	}
	talloc_free(data);

	return true;
}
