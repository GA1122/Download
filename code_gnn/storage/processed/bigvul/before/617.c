bool asn1_push_tag(struct asn1_data *data, uint8_t tag)
{
	struct nesting *nesting;

	asn1_write_uint8(data, tag);
	nesting = talloc(data, struct nesting);
	if (!nesting) {
		data->has_error = true;
		return false;
	}

	nesting->start = data->ofs;
	nesting->next = data->nesting;
	data->nesting = nesting;
	return asn1_write_uint8(data, 0xff);
}
