json_in(PG_FUNCTION_ARGS)
{
	char	   *json = PG_GETARG_CSTRING(0);
	text	   *result = cstring_to_text(json);
	JsonLexContext *lex;

	 
	lex = makeJsonLexContext(result, false);
	pg_parse_json(lex, &nullSemAction);

	 
	PG_RETURN_TEXT_P(result);
}
