static bool decode_asq_control(void *mem_ctx, DATA_BLOB in, void *_out)
{
	void **out = (void **)_out;
	DATA_BLOB source_attribute;
	struct asn1_data *data = asn1_init(mem_ctx);
	struct ldb_asq_control *lac;

	if (!data) return false;

	if (!asn1_load(data, in)) {
		return false;
	}

	lac = talloc(mem_ctx, struct ldb_asq_control);
	if (!lac) {
		return false;
	}

	if (!asn1_start_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (asn1_peek_tag(data, ASN1_OCTET_STRING)) {

		if (!asn1_read_OctetString(data, mem_ctx, &source_attribute)) {
			return false;
		}
		lac->src_attr_len = source_attribute.length;
		if (lac->src_attr_len) {
			lac->source_attribute = talloc_strndup(lac, (const char *)source_attribute.data, source_attribute.length);

			if (!(lac->source_attribute)) {
				return false;
			}
		} else {
			lac->source_attribute = NULL;
		}

		lac->request = 1;

	} else if (asn1_peek_tag(data, ASN1_ENUMERATED)) {

		if (!asn1_read_enumerated(data, &(lac->result))) {
			return false;
		}

		lac->request = 0;

	} else {
		return false;
	}

	if (!asn1_end_tag(data)) {
		return false;
	}

	*out = lac;

	return true;
}
