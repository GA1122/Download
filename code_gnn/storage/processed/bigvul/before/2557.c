to_json(PG_FUNCTION_ARGS)
{
	Datum		val = PG_GETARG_DATUM(0);
	Oid			val_type = get_fn_expr_argtype(fcinfo->flinfo, 0);
	StringInfo	result;
	JsonTypeCategory tcategory;
	Oid			outfuncoid;

	if (val_type == InvalidOid)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("could not determine input data type")));

	json_categorize_type(val_type,
						 &tcategory, &outfuncoid);

	result = makeStringInfo();

	datum_to_json(val, false, result, tcategory, outfuncoid, false);

	PG_RETURN_TEXT_P(cstring_to_text_with_len(result->data, result->len));
}
