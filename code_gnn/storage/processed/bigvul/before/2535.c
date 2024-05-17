json_object(PG_FUNCTION_ARGS)
{
	ArrayType  *in_array = PG_GETARG_ARRAYTYPE_P(0);
	int			ndims = ARR_NDIM(in_array);
	StringInfoData result;
	Datum	   *in_datums;
	bool	   *in_nulls;
	int			in_count,
				count,
				i;
	text	   *rval;
	char	   *v;

	switch (ndims)
	{
		case 0:
			PG_RETURN_DATUM(CStringGetTextDatum("{}"));
			break;

		case 1:
			if ((ARR_DIMS(in_array)[0]) % 2)
				ereport(ERROR,
						(errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
						 errmsg("array must have even number of elements")));
			break;

		case 2:
			if ((ARR_DIMS(in_array)[1]) != 2)
				ereport(ERROR,
						(errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
						 errmsg("array must have two columns")));
			break;

		default:
			ereport(ERROR,
					(errcode(ERRCODE_ARRAY_SUBSCRIPT_ERROR),
					 errmsg("wrong number of array subscripts")));
	}

	deconstruct_array(in_array,
					  TEXTOID, -1, false, 'i',
					  &in_datums, &in_nulls, &in_count);

	count = in_count / 2;

	initStringInfo(&result);

	appendStringInfoChar(&result, '{');

	for (i = 0; i < count; ++i)
	{
		if (in_nulls[i * 2])
			ereport(ERROR,
					(errcode(ERRCODE_NULL_VALUE_NOT_ALLOWED),
					 errmsg("null value not allowed for object key")));

		v = TextDatumGetCString(in_datums[i * 2]);
		if (i > 0)
			appendStringInfoString(&result, ", ");
		escape_json(&result, v);
		appendStringInfoString(&result, " : ");
		pfree(v);
		if (in_nulls[i * 2 + 1])
			appendStringInfoString(&result, "null");
		else
		{
			v = TextDatumGetCString(in_datums[i * 2 + 1]);
			escape_json(&result, v);
			pfree(v);
		}
	}

	appendStringInfoChar(&result, '}');

	pfree(in_datums);
	pfree(in_nulls);

	rval = cstring_to_text_with_len(result.data, result.len);
	pfree(result.data);

	PG_RETURN_TEXT_P(rval);

}
