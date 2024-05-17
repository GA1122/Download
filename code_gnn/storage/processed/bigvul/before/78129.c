sc_asn1_encode_object_id(u8 **buf, size_t *buflen, const struct sc_object_id *id)
{
	u8 temp[SC_MAX_OBJECT_ID_OCTETS*5], *p = temp;
	int	i;

	if (!buflen || !id)
		return SC_ERROR_INVALID_ARGUMENTS;

	 
	if (id->value[0] == -1 || id->value[1] == -1)
		return SC_ERROR_INVALID_ARGUMENTS;

	for (i = 0; i < SC_MAX_OBJECT_ID_OCTETS; i++) {
		unsigned int k, shift;

		if (id->value[i] == -1)
			break;

		k = id->value[i];
		switch (i) {
		case 0:
			if (k > 2)
				return SC_ERROR_INVALID_ARGUMENTS;
			*p = k * 40;
			break;
		case 1:
			if (k > 39)
				return SC_ERROR_INVALID_ARGUMENTS;
			*p++ += k;
			break;
		default:
			shift = 28;
			while (shift && (k >> shift) == 0)
				shift -= 7;
			while (shift) {
				*p++ = 0x80 | ((k >> shift) & 0x7f);
				shift -= 7;
			}
			*p++ = k & 0x7F;
			break;
		}
	}

	*buflen = p - temp;

	if (buf)   {
		*buf = malloc(*buflen);
		if (!*buf)
			return SC_ERROR_OUT_OF_MEMORY;
		memcpy(*buf, temp, *buflen);
	}
	return 0;
}