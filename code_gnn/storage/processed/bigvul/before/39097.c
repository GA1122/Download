bitsubstring(VarBit *arg, int32 s, int32 l, bool length_not_specified)
{
	VarBit	   *result;
	int			bitlen,
				rbitlen,
				len,
				ipad = 0,
				ishift,
				i;
	int			e,
				s1,
				e1;
	bits8		mask,
			   *r,
			   *ps;

	bitlen = VARBITLEN(arg);
	s1 = Max(s, 1);
	 
	if (length_not_specified)
	{
		e1 = bitlen + 1;
	}
	else
	{
		e = s + l;

		 
		if (e < s)
			ereport(ERROR,
					(errcode(ERRCODE_SUBSTRING_ERROR),
					 errmsg("negative substring length not allowed")));
		e1 = Min(e, bitlen + 1);
	}
	if (s1 > bitlen || e1 <= s1)
	{
		 
		len = VARBITTOTALLEN(0);
		result = (VarBit *) palloc(len);
		SET_VARSIZE(result, len);
		VARBITLEN(result) = 0;
	}
	else
	{
		 
		rbitlen = e1 - s1;
		len = VARBITTOTALLEN(rbitlen);
		result = (VarBit *) palloc(len);
		SET_VARSIZE(result, len);
		VARBITLEN(result) = rbitlen;
		len -= VARHDRSZ + VARBITHDRSZ;
		 
		if ((s1 - 1) % BITS_PER_BYTE == 0)
		{
			 
			memcpy(VARBITS(result), VARBITS(arg) + (s1 - 1) / BITS_PER_BYTE,
				   len);
		}
		else
		{
			 
			ishift = (s1 - 1) % BITS_PER_BYTE;
			r = VARBITS(result);
			ps = VARBITS(arg) + (s1 - 1) / BITS_PER_BYTE;
			for (i = 0; i < len; i++)
			{
				*r = (*ps << ishift) & BITMASK;
				if ((++ps) < VARBITEND(arg))
					*r |= *ps >> (BITS_PER_BYTE - ishift);
				r++;
			}
		}
		 
		ipad = VARBITPAD(result);
		if (ipad > 0)
		{
			mask = BITMASK << ipad;
			*(VARBITS(result) + len - 1) &= mask;
		}
	}

	return result;
}
