static bool encode_extended_dn_request(void *mem_ctx, void *in, DATA_BLOB *out)
{
	struct ldb_extended_dn_control *ledc = talloc_get_type(in, struct ldb_extended_dn_control);
	struct asn1_data *data;

	if (!in) {
		*out = data_blob(NULL, 0);
		return true;
	}

	data = asn1_init(mem_ctx);

	if (!data) return false;

	if (!asn1_push_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_write_Integer(data, ledc->type)) {
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
