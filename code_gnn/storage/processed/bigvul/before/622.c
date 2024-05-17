bool asn1_write_implicit_Integer(struct asn1_data *data, int i)
{
	if (i == -1) {
		 
		return asn1_write_uint8(data, 0xff);
	} else {
		return push_int_bigendian(data, i, i<0);
	}
}
