jsonb_object_field(PG_FUNCTION_ARGS)
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
		PG_RETURN_JSONB(JsonbValueToJsonb(v));

	PG_RETURN_NULL();
}
