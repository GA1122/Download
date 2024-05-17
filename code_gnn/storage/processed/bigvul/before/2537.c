json_object_agg_transfn(PG_FUNCTION_ARGS)
{
	MemoryContext aggcontext,
				oldcontext;
	JsonAggState	*state;
	Datum		arg;

	if (!AggCheckCallContext(fcinfo, &aggcontext))
	{
		 
		elog(ERROR, "json_object_agg_transfn called in non-aggregate context");
	}

	if (PG_ARGISNULL(0))
	{
		Oid			arg_type;

		 
		oldcontext = MemoryContextSwitchTo(aggcontext);
		state = (JsonAggState *) palloc(sizeof(JsonAggState));
		state->str = makeStringInfo();
		MemoryContextSwitchTo(oldcontext);

		arg_type = get_fn_expr_argtype(fcinfo->flinfo, 1);

		if (arg_type == InvalidOid)
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("could not determine data type for argument 1")));

		json_categorize_type(arg_type,&state->key_category,
							 &state->key_output_func);

		arg_type = get_fn_expr_argtype(fcinfo->flinfo, 2);

		if (arg_type == InvalidOid)
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("could not determine data type for argument 2")));

		json_categorize_type(arg_type,&state->val_category,
							 &state->val_output_func);

		appendStringInfoString(state->str, "{ ");
	}
	else
	{
		state = (JsonAggState *) PG_GETARG_POINTER(0);
		appendStringInfoString(state->str, ", ");
	}

	 

	if (PG_ARGISNULL(1))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("field name must not be null")));

	arg = PG_GETARG_DATUM(1);

	datum_to_json(arg, false, state->str, state->key_category,
				  state->key_output_func, true);

	appendStringInfoString(state->str, " : ");

	if (PG_ARGISNULL(2))
		arg = (Datum) 0;
	else
		arg = PG_GETARG_DATUM(2);

	datum_to_json(arg, PG_ARGISNULL(2), state->str, state->val_category,
				  state->val_output_func, false);

	PG_RETURN_POINTER(state);
}

 
Datum
json_object_agg_finalfn(PG_FUNCTION_ARGS)
{
	JsonAggState	*state;

	 
	Assert(AggCheckCallContext(fcinfo, NULL));

	state = PG_ARGISNULL(0) ? NULL : (JsonAggState *) PG_GETARG_POINTER(0);

	 
	if (state == NULL)
		PG_RETURN_NULL();

	 
	PG_RETURN_TEXT_P(catenate_stringinfo_string(state->str, " }"));
}
