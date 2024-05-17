bool asn1_read_OctetString_talloc(TALLOC_CTX *mem_ctx,
				  struct asn1_data *data,
				  const char **result)
{
	DATA_BLOB string;
	if (!asn1_read_OctetString(data, mem_ctx, &string))
		return false;
	*result = blob2string_talloc(mem_ctx, string);
	data_blob_free(&string);
	return true;
}
