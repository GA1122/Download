bool asn1_write_uint8(struct asn1_data *data, uint8_t v)
{
	return asn1_write(data, &v, 1);
}
