hstore_hash(PG_FUNCTION_ARGS)
{
	HStore	   *hs = PG_GETARG_HS(0);
	Datum		hval = hash_any((unsigned char *) VARDATA(hs),
								VARSIZE(hs) - VARHDRSZ);

	 
	Assert(VARSIZE(hs) ==
		   (HS_COUNT(hs) != 0 ?
			CALCDATASIZE(HS_COUNT(hs),
						 HSE_ENDPOS(ARRPTR(hs)[2 * HS_COUNT(hs) - 1])) :
			HSHRDSIZE));

	PG_FREE_IF_COPY(hs, 0);
	PG_RETURN_DATUM(hval);
}
