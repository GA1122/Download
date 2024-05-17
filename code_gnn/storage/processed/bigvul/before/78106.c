static int asn1_encode_integer(int in, u8 ** obj, size_t * objsize)
{
	int i = sizeof(in) * 8, skip_zero, skip_sign;
	u8 *p, b;

	if (in < 0)
	{
		skip_sign = 1;
		skip_zero= 0;
	}
	else
	{
		skip_sign = 0;
		skip_zero= 1;
	}
	*obj = p = malloc(sizeof(in)+1);
	if (*obj == NULL)
		return SC_ERROR_OUT_OF_MEMORY;
	do {
		i -= 8;
		b = in >> i;
		if (skip_sign)
		{
			if (b != 0xff)
				skip_sign = 0;
			if (b & 0x80)
			{
				*p = b;
				if (0xff == b)
					continue;
			}
			else
			{
				p++;
				skip_sign = 0;
			}
		}
		if (b == 0 && skip_zero)
			continue;
		if (skip_zero) {
			skip_zero = 0;
			 
			if ((b & 0x80) != 0 && in > 0)
				*p++ = 0;
		}
		*p++ = b;
	} while (i > 0);
	if (skip_sign)
		p++;
	*objsize = p - *obj;
	if (*objsize == 0) {
		*objsize = 1;
		(*obj)[0] = 0;
	}
	return 0;
}