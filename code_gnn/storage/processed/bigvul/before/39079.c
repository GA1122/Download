bitfromint8(PG_FUNCTION_ARGS)
{
	int64		a = PG_GETARG_INT64(0);
	int32		typmod = PG_GETARG_INT32(1);
	VarBit	   *result;
	bits8	   *r;
	int			rlen;
	int			destbitsleft,
				srcbitsleft;

	if (typmod <= 0)
		typmod = 1;				 

	rlen = VARBITTOTALLEN(typmod);
	result = (VarBit *) palloc(rlen);
	SET_VARSIZE(result, rlen);
	VARBITLEN(result) = typmod;

	r = VARBITS(result);
	destbitsleft = typmod;
	srcbitsleft = 64;
	 
	srcbitsleft = Min(srcbitsleft, destbitsleft);
	 
	while (destbitsleft >= srcbitsleft + 8)
	{
		*r++ = (bits8) ((a < 0) ? BITMASK : 0);
		destbitsleft -= 8;
	}
	 
	if (destbitsleft > srcbitsleft)
	{
		int			val = (int) (a >> (destbitsleft - 8));

		 
		if (a < 0)
			val |= (-1) << (srcbitsleft + 8 - destbitsleft);
		*r++ = (bits8) (val & BITMASK);
		destbitsleft -= 8;
	}
	 
	 
	while (destbitsleft >= 8)
	{
		*r++ = (bits8) ((a >> (destbitsleft - 8)) & BITMASK);
		destbitsleft -= 8;
	}
	 
	if (destbitsleft > 0)
		*r = (bits8) ((a << (8 - destbitsleft)) & BITMASK);

	PG_RETURN_VARBIT_P(result);
}
