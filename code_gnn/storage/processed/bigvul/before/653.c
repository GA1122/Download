static bool encode_server_sort_request(void *mem_ctx, void *in, DATA_BLOB *out)
{
	struct ldb_server_sort_control **lssc = talloc_get_type(in, struct ldb_server_sort_control *);
	struct asn1_data *data = asn1_init(mem_ctx);
	int num;

	if (!data) return false;

	if (!asn1_push_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	 
	for (num = 0; lssc[num]; num++) {
		if (!asn1_push_tag(data, ASN1_SEQUENCE(0))) {
			return false;
		}
		
		if (!asn1_write_OctetString(data, lssc[num]->attributeName, strlen(lssc[num]->attributeName))) {
			return false;
		}

		if (lssc[num]->orderingRule) {
			if (!asn1_write_OctetString(data, lssc[num]->orderingRule, strlen(lssc[num]->orderingRule))) {
				return false;
			}
		}

		if (lssc[num]->reverse) {
			if (!asn1_write_BOOLEAN_context(data, lssc[num]->reverse, 1)) {
				return false;
			}
		}

		if (!asn1_pop_tag(data)) {
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
