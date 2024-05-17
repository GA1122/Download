jsonb_array_element_text(PG_FUNCTION_ARGS)
{
	Jsonb	   *jb = PG_GETARG_JSONB(0);
	int			element = PG_GETARG_INT32(1);
	JsonbValue *v;

	if (!JB_ROOT_IS_ARRAY(jb))
		PG_RETURN_NULL();

	 
	if (element < 0)
	{
		uint32	nelements = JB_ROOT_COUNT(jb);

		if (-element > nelements)
			PG_RETURN_NULL();
		else
			element += nelements;
	}

	v = getIthJsonbValueFromContainer(&jb->root, element);
	if (v != NULL)
	{
		text	   *result = NULL;

		switch (v->type)
		{
			case jbvNull:
				break;
			case jbvBool:
				result = cstring_to_text(v->val.boolean ? "true" : "false");
				break;
			case jbvString:
				result = cstring_to_text_with_len(v->val.string.val, v->val.string.len);
				break;
			case jbvNumeric:
				result = cstring_to_text(DatumGetCString(DirectFunctionCall1(numeric_out,
										  PointerGetDatum(v->val.numeric))));
				break;
			case jbvBinary:
				{
					StringInfo	jtext = makeStringInfo();

					(void) JsonbToCString(jtext, v->val.binary.data, -1);
					result = cstring_to_text_with_len(jtext->data, jtext->len);
				}
				break;
			default:
				elog(ERROR, "unrecognized jsonb type: %d", (int) v->type);
		}

		if (result)
			PG_RETURN_TEXT_P(result);
	}

	PG_RETURN_NULL();
}
