bool asn1_pop_tag(struct asn1_data *data)
{
	struct nesting *nesting;
	size_t len;

	nesting = data->nesting;

	if (!nesting) {
		data->has_error = true;
		return false;
	}
	len = data->ofs - (nesting->start+1);
	 
	if (len > 0xFFFFFF) {
		data->data[nesting->start] = 0x84;
		if (!asn1_write_uint8(data, 0)) return false;
		if (!asn1_write_uint8(data, 0)) return false;
		if (!asn1_write_uint8(data, 0)) return false;
		if (!asn1_write_uint8(data, 0)) return false;
		memmove(data->data+nesting->start+5, data->data+nesting->start+1, len);
		data->data[nesting->start+1] = (len>>24) & 0xFF;
		data->data[nesting->start+2] = (len>>16) & 0xFF;
		data->data[nesting->start+3] = (len>>8) & 0xFF;
		data->data[nesting->start+4] = len&0xff;
	} else if (len > 0xFFFF) {
		data->data[nesting->start] = 0x83;
		if (!asn1_write_uint8(data, 0)) return false;
		if (!asn1_write_uint8(data, 0)) return false;
		if (!asn1_write_uint8(data, 0)) return false;
		memmove(data->data+nesting->start+4, data->data+nesting->start+1, len);
		data->data[nesting->start+1] = (len>>16) & 0xFF;
		data->data[nesting->start+2] = (len>>8) & 0xFF;
		data->data[nesting->start+3] = len&0xff;
	} else if (len > 255) {
		data->data[nesting->start] = 0x82;
		if (!asn1_write_uint8(data, 0)) return false;
		if (!asn1_write_uint8(data, 0)) return false;
		memmove(data->data+nesting->start+3, data->data+nesting->start+1, len);
		data->data[nesting->start+1] = len>>8;
		data->data[nesting->start+2] = len&0xff;
	} else if (len > 127) {
		data->data[nesting->start] = 0x81;
		if (!asn1_write_uint8(data, 0)) return false;
		memmove(data->data+nesting->start+2, data->data+nesting->start+1, len);
		data->data[nesting->start+1] = len;
	} else {
		data->data[nesting->start] = len;
	}

	data->nesting = nesting->next;
	talloc_free(nesting);
	return true;
}
