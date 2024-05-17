bit(PG_FUNCTION_ARGS)
{
	VarBit	   *arg = PG_GETARG_VARBIT_P(0);
	int32		len = PG_GETARG_INT32(1);
	bool		isExplicit = PG_GETARG_BOOL(2);
	VarBit	   *result;
	int			rlen;
	int			ipad;
	bits8		mask;

	 
	if (len <= 0 || len == VARBITLEN(arg))
		PG_RETURN_VARBIT_P(arg);

	if (!isExplicit)
		ereport(ERROR,
				(errcode(ERRCODE_STRING_DATA_LENGTH_MISMATCH),
				 errmsg("bit string length %d does not match type bit(%d)",
						VARBITLEN(arg), len)));

	rlen = VARBITTOTALLEN(len);
	 
	result = (VarBit *) palloc0(rlen);
	SET_VARSIZE(result, rlen);
	VARBITLEN(result) = len;

	memcpy(VARBITS(result), VARBITS(arg),
		   Min(VARBITBYTES(result), VARBITBYTES(arg)));

	 
	ipad = VARBITPAD(result);
	if (ipad > 0)
	{
		mask = BITMASK << ipad;
		*(VARBITS(result) + VARBITBYTES(result) - 1) &= mask;
	}

	PG_RETURN_VARBIT_P(result);
}
