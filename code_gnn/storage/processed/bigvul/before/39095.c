bitsubstr(PG_FUNCTION_ARGS)
{
	PG_RETURN_VARBIT_P(bitsubstring(PG_GETARG_VARBIT_P(0),
									PG_GETARG_INT32(1),
									PG_GETARG_INT32(2),
									false));
}