static bool decode_vlv_request(void *mem_ctx, DATA_BLOB in, void *_out)
{
	void **out = (void **)_out;
	DATA_BLOB assertion_value, context_id;
	struct asn1_data *data = asn1_init(mem_ctx);
	struct ldb_vlv_req_control *lvrc;

	if (!data) return false;

	if (!asn1_load(data, in)) {
		return false;
	}

	lvrc = talloc(mem_ctx, struct ldb_vlv_req_control);
	if (!lvrc) {
		return false;
	}

	if (!asn1_start_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_read_Integer(data, &(lvrc->beforeCount))) {
		return false;
	}
	
	if (!asn1_read_Integer(data, &(lvrc->afterCount))) {
		return false;
	}

	if (asn1_peek_tag(data, ASN1_CONTEXT(0))) {

		lvrc->type = 0;
		
		if (!asn1_start_tag(data, ASN1_CONTEXT(0))) {
			return false;
		}

		if (!asn1_start_tag(data, ASN1_SEQUENCE(0))) {
			return false;
		}

		if (!asn1_read_Integer(data, &(lvrc->match.byOffset.offset))) {
			return false;
		}

		if (!asn1_read_Integer(data, &(lvrc->match.byOffset.contentCount))) {
			return false;
		}

		if (!asn1_end_tag(data)) {  
			return false;
		}

		if (!asn1_end_tag(data)) {  
			return false;
		}

	} else {

		lvrc->type = 1;

		if (!asn1_start_tag(data, ASN1_CONTEXT(1))) {
			return false;
		}

		if (!asn1_read_OctetString(data, mem_ctx, &assertion_value)) {
			return false;
		}
		lvrc->match.gtOrEq.value_len = assertion_value.length;
		if (lvrc->match.gtOrEq.value_len) {
			lvrc->match.gtOrEq.value = talloc_memdup(lvrc, assertion_value.data, assertion_value.length);

			if (!(lvrc->match.gtOrEq.value)) {
				return false;
			}
		} else {
			lvrc->match.gtOrEq.value = NULL;
		}

		if (!asn1_end_tag(data)) {  
			return false;
		}
	}

	if (asn1_peek_tag(data, ASN1_OCTET_STRING)) {
		if (!asn1_read_OctetString(data, mem_ctx, &context_id)) {
			return false;
		}
		lvrc->ctxid_len = context_id.length;
		if (lvrc->ctxid_len) {
			lvrc->contextId = talloc_memdup(lvrc, context_id.data, context_id.length);

			if (!(lvrc->contextId)) {
				return false;
			}
		} else {
			lvrc->contextId = NULL;
		}
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
