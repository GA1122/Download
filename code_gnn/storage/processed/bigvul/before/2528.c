json_build_object(PG_FUNCTION_ARGS)
{
	int			nargs = PG_NARGS();
	int			i;
	Datum		arg;
	const char *sep = "";
	StringInfo	result;
	Oid			val_type;

	if (nargs % 2 != 0)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("argument list must have even number of elements"),
				 errhint("The arguments of json_build_object() must consist of alternating keys and values.")));

	result = makeStringInfo();

	appendStringInfoChar(result, '{');

	for (i = 0; i < nargs; i += 2)
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
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("argument %d cannot be null", i + 1),
					 errhint("Object keys should be text.")));

		arg = PG_GETARG_DATUM(i);

		add_json(arg, false, result, val_type, true);

		appendStringInfoString(result, " : ");

		 
		val_type = get_fn_expr_argtype(fcinfo->flinfo, i + 1);

		if (val_type == InvalidOid)
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("could not determine data type for argument %d",
							i + 2)));

		if (PG_ARGISNULL(i + 1))
			arg = (Datum) 0;
		else
			arg = PG_GETARG_DATUM(i + 1);

		add_json(arg, PG_ARGISNULL(i + 1), result, val_type, false);
	}

	appendStringInfoChar(result, '}');

	PG_RETURN_TEXT_P(cstring_to_text_with_len(result->data, result->len));
}
