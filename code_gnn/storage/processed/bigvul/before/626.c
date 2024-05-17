static bool push_int_bigendian(struct asn1_data *data, unsigned int i, bool negative)
{
	uint8_t lowest = i & 0xFF;

	i = i >> 8;
	if (i != 0)
		if (!push_int_bigendian(data, i, negative))
			return false;

	if (data->nesting->start+1 == data->ofs) {

		 

		if (negative) {
			 
			if (lowest == 0xFF)
				return true;

			if ((lowest & 0x80) == 0) {
				 
				if (!asn1_write_uint8(data, 0xff))
					return false;
			}
		} else {
			if (lowest & 0x80) {
				 
				if (!asn1_write_uint8(data, 0))
					return false;
			}
		}
	}

	return asn1_write_uint8(data, lowest);
}
