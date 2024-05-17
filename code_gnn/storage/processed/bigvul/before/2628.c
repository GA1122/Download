jsonb_object_field_text(PG_FUNCTION_ARGS)
{
	Jsonb	   *jb = PG_GETARG_JSONB(0);
	text	   *key = PG_GETARG_TEXT_PP(1);
	JsonbValue *v;

	if (!JB_ROOT_IS_OBJECT(jb))
		PG_RETURN_NULL();

	v = findJsonbValueFromContainerLen(&jb->root, JB_FOBJECT,
									   VARDATA_ANY(key),
									   VARSIZE_ANY_EXHDR(key));

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
