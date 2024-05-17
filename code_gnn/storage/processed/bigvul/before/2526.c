json_build_array(PG_FUNCTION_ARGS)
{
	int			nargs = PG_NARGS();
	int			i;
	Datum		arg;
	const char *sep = "";
	StringInfo	result;
	Oid			val_type;

	result = makeStringInfo();

	appendStringInfoChar(result, '[');

	for (i = 0; i < nargs; i++)
	{
		 
		appendStringInfoString(result, sep);
		sep = ", ";

		val_type = get_fn_expr_argtype(fcinfo->flinfo, i);

		if (val_type == InvalidOid)
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("could not determine data type for argument %d",
							i + 1)));

		if (PG_ARGISNULL(i))
			arg = (Datum) 0;
		else
			arg = PG_GETARG_DATUM(i);

		add_json(arg, PG_ARGISNULL(i), result, val_type, false);
	}

	appendStringInfoChar(result, ']');

	PG_RETURN_TEXT_P(cstring_to_text_with_len(result->data, result->len));
}
