static bool encode_vlv_request(void *mem_ctx, void *in, DATA_BLOB *out)
{
	struct ldb_vlv_req_control *lvrc = talloc_get_type(in, struct ldb_vlv_req_control);
	struct asn1_data *data = asn1_init(mem_ctx);

	if (!data) return false;

	if (!asn1_push_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_write_Integer(data, lvrc->beforeCount)) {
		return false;
	}

	if (!asn1_write_Integer(data, lvrc->afterCount)) {
		return false;
	}

	if (lvrc->type == 0) {
		if (!asn1_push_tag(data, ASN1_CONTEXT(0))) {
			return false;
		}
		
		if (!asn1_push_tag(data, ASN1_SEQUENCE(0))) {
			return false;
		}
		
		if (!asn1_write_Integer(data, lvrc->match.byOffset.offset)) {
			return false;
		}

		if (!asn1_write_Integer(data, lvrc->match.byOffset.contentCount)) {
			return false;
		}

		if (!asn1_pop_tag(data)) {  
			return false;
		}

		if (!asn1_pop_tag(data)) {  
			return false;
		}
	} else {
		if (!asn1_push_tag(data, ASN1_CONTEXT(1))) {
			return false;
		}
		
		if (!asn1_write_OctetString(data, lvrc->match.gtOrEq.value, lvrc->match.gtOrEq.value_len)) {
			return false;
		}

		if (!asn1_pop_tag(data)) {  
			return false;
		}
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
