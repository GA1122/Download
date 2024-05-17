bool asn1_write(struct asn1_data *data, const void *p, int len)
{
	if (data->has_error) return false;
	if (data->length < data->ofs+len) {
		uint8_t *newp;
		newp = talloc_realloc(data, data->data, uint8_t, data->ofs+len);
		if (!newp) {
			asn1_free(data);
			data->has_error = true;
			return false;
		}
		data->data = newp;
		data->length = data->ofs+len;
	}
	memcpy(data->data + data->ofs, p, len);
	data->ofs += len;
	return true;
}
