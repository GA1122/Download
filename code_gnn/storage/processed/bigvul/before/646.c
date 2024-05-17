static bool decode_vlv_response(void *mem_ctx, DATA_BLOB in, void *_out)
{
	void **out = (void **)_out;
	DATA_BLOB context_id;
	struct asn1_data *data = asn1_init(mem_ctx);
	struct ldb_vlv_resp_control *lvrc;

	if (!data) return false;

	if (!asn1_load(data, in)) {
		return false;
	}

	lvrc = talloc(mem_ctx, struct ldb_vlv_resp_control);
	if (!lvrc) {
		return false;
	}

	if (!asn1_start_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_read_Integer(data, &(lvrc->targetPosition))) {
		return false;
	}
	
	if (!asn1_read_Integer(data, &(lvrc->contentCount))) {
		return false;
	}
	
	if (!asn1_read_enumerated(data, &(lvrc->vlv_result))) {
		return false;
	}

	if (asn1_peek_tag(data, ASN1_OCTET_STRING)) {
		if (!asn1_read_OctetString(data, mem_ctx, &context_id)) {
			return false;
		}
		lvrc->contextId = talloc_strndup(lvrc, (const char *)context_id.data, context_id.length);
		if (!lvrc->contextId) {
			return false;
		}
		lvrc->ctxid_len = context_id.length;
	} else {
		lvrc->contextId = NULL;
		lvrc->ctxid_len = 0;
	}

	if (!asn1_end_tag(data)) {
		return false;
	}

	*out = lvrc;

	return true;
}
