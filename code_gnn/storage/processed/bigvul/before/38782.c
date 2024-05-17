rboolop(PG_FUNCTION_ARGS)
{
	 
	return DirectFunctionCall2(boolop,
							   PG_GETARG_DATUM(1),
							   PG_GETARG_DATUM(0));
}
