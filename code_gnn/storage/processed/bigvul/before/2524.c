json_agg_finalfn(PG_FUNCTION_ARGS)
{
	JsonAggState	*state;

	 
	Assert(AggCheckCallContext(fcinfo, NULL));

	state = PG_ARGISNULL(0) ?
		NULL :
		(JsonAggState *) PG_GETARG_POINTER(0);

	 
	if (state == NULL)
		PG_RETURN_NULL();

	 
	PG_RETURN_TEXT_P(catenate_stringinfo_string(state->str, "]"));
}
