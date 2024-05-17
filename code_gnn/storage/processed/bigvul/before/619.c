bool asn1_write_BitString(struct asn1_data *data, const void *p, size_t length, uint8_t padding)
{
	if (!asn1_push_tag(data, ASN1_BIT_STRING)) return false;
	if (!asn1_write_uint8(data, padding)) return false;
	if (!asn1_write(data, p, length)) return false;
	return asn1_pop_tag(data);
}
