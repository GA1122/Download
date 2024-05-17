static bool encode_server_sort_response(void *mem_ctx, void *in, DATA_BLOB *out)
{
	struct ldb_sort_resp_control *lsrc = talloc_get_type(in, struct ldb_sort_resp_control);
	struct asn1_data *data = asn1_init(mem_ctx);

	if (!data) return false;

	if (!asn1_push_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_write_enumerated(data, lsrc->result)) {
		return false;
	}

	if (lsrc->attr_desc) {
		if (!asn1_write_OctetString(data, lsrc->attr_desc, strlen(lsrc->attr_desc))) {
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
