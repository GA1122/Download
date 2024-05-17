bool asn1_write_OID(struct asn1_data *data, const char *OID)
{
	DATA_BLOB blob;

	if (!asn1_push_tag(data, ASN1_OID)) return false;

	if (!ber_write_OID_String(NULL, &blob, OID)) {
		data->has_error = true;
		return false;
	}

	if (!asn1_write(data, blob.data, blob.length)) {
		data_blob_free(&blob);
		data->has_error = true;
		return false;
	}
	data_blob_free(&blob);
	return asn1_pop_tag(data);
}
