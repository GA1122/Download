static bool encode_verify_name_request(void *mem_ctx, void *in, DATA_BLOB *out)
{
	struct ldb_verify_name_control *lvnc = talloc_get_type(in, struct ldb_verify_name_control);
	struct asn1_data *data = asn1_init(mem_ctx);
	DATA_BLOB gc_utf16;

	if (!data) return false;

	if (!asn1_push_tag(data, ASN1_SEQUENCE(0))) {
		return false;
	}

	if (!asn1_write_Integer(data, lvnc->flags)) {
		return false;
	}

	if (lvnc->gc_len) {
		convert_string_talloc(mem_ctx, CH_UNIX, CH_UTF16,
						lvnc->gc, lvnc->gc_len,
						(void **)&gc_utf16.data, &gc_utf16.length);
		if (!asn1_write_OctetString(data, gc_utf16.data, gc_utf16.length)) {
			return false;
		}
	} else {
		if (!asn1_write_OctetString(data, NULL, 0)) {
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
