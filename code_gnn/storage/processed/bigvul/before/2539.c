json_out(PG_FUNCTION_ARGS)
{
	 
	Datum		txt = PG_GETARG_DATUM(0);

	PG_RETURN_CSTRING(TextDatumGetCString(txt));
}
