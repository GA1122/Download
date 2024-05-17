bool asn1_write_Integer(struct asn1_data *data, int i)
{
	if (!asn1_push_tag(data, ASN1_INTEGER)) return false;
	if (!asn1_write_implicit_Integer(data, i)) return false;
	return asn1_pop_tag(data);
}
