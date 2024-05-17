json_typeof(PG_FUNCTION_ARGS)
{
	text	   *json;

	JsonLexContext *lex;
	JsonTokenType tok;
	char	   *type;

	json = PG_GETARG_TEXT_P(0);
	lex = makeJsonLexContext(json, false);

	 
	json_lex(lex);
	tok = lex_peek(lex);
	switch (tok)
	{
		case JSON_TOKEN_OBJECT_START:
			type = "object";
			break;
		case JSON_TOKEN_ARRAY_START:
			type = "array";
			break;
		case JSON_TOKEN_STRING:
			type = "string";
			break;
		case JSON_TOKEN_NUMBER:
			type = "number";
			break;
		case JSON_TOKEN_TRUE:
		case JSON_TOKEN_FALSE:
			type = "boolean";
			break;
		case JSON_TOKEN_NULL:
			type = "null";
			break;
		default:
			elog(ERROR, "unexpected json token: %d", tok);
	}

	PG_RETURN_TEXT_P(cstring_to_text(type));
}
