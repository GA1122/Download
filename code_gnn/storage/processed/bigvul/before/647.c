static bool encode_asq_control(void *mem_ctx, void *in, DATA_BLOB *out)
{
	struct ldb_asq_control *lac = talloc_get_type(in, struct ldb_asq_control);
	struct asn1_data *data = asn1_init(mem_ctx);

	if (!data) return false;

	if (!asn1_push_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (lac->request) {

		if (!asn1_write_OctetString(data, lac->source_attribute, lac->src_attr_len)) {
			return false;
		}
	} else {
		if (!asn1_write_enumerated(data, lac->result)) {
			return false;
		}
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
