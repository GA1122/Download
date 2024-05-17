static bool encode_vlv_response(void *mem_ctx, void *in, DATA_BLOB *out)
{
	struct ldb_vlv_resp_control *lvrc = talloc_get_type(in, struct ldb_vlv_resp_control);
	struct asn1_data *data = asn1_init(mem_ctx);

	if (!data) return false;

	if (!asn1_push_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_write_Integer(data, lvrc->targetPosition)) {
		return false;
	}

	if (!asn1_write_Integer(data, lvrc->contentCount)) {
		return false;
	}

	if (!asn1_write_enumerated(data, lvrc->vlv_result)) {
		return false;
	}

	if (lvrc->ctxid_len) {
		if (!asn1_write_OctetString(data, lvrc->contextId, lvrc->ctxid_len)) {
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
