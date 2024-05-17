json_agg_transfn(PG_FUNCTION_ARGS)
{
	MemoryContext aggcontext,
				oldcontext;
	JsonAggState	*state;
	Datum		val;

	if (!AggCheckCallContext(fcinfo, &aggcontext))
	{
		 
		elog(ERROR, "json_agg_transfn called in non-aggregate context");
	}

	if (PG_ARGISNULL(0))
	{
		Oid         arg_type = get_fn_expr_argtype(fcinfo->flinfo, 1);

		if (arg_type == InvalidOid)
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("could not determine input data type")));

		 
		oldcontext = MemoryContextSwitchTo(aggcontext);
		state = (JsonAggState *) palloc(sizeof(JsonAggState));
		state->str = makeStringInfo();
		MemoryContextSwitchTo(oldcontext);

		appendStringInfoChar(state->str, '[');
		json_categorize_type(arg_type,&state->val_category,
							 &state->val_output_func);
	}
	else
	{
		state = (JsonAggState *) PG_GETARG_POINTER(0);
		appendStringInfoString(state->str, ", ");
	}

	 
	if (PG_ARGISNULL(1))
	{
		datum_to_json((Datum) 0, true, state->str, JSONTYPE_NULL,
					  InvalidOid, false);
		PG_RETURN_POINTER(state);
	}

	val = PG_GETARG_DATUM(1);

	 
	if (!PG_ARGISNULL(0) &&
		(state->val_category == JSONTYPE_ARRAY ||
		 state->val_category == JSONTYPE_COMPOSITE))
	{
		appendStringInfoString(state->str, "\n ");
	}

	datum_to_json(val, false, state->str, state->val_category,
				  state->val_output_func, false);

	 
	PG_RETURN_POINTER(state);
}
