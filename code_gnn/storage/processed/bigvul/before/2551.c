pg_parse_json(JsonLexContext *lex, JsonSemAction *sem)
{
	JsonTokenType tok;

	 
	json_lex(lex);

	tok = lex_peek(lex);

	 
	switch (tok)
	{
		case JSON_TOKEN_OBJECT_START:
			parse_object(lex, sem);
			break;
		case JSON_TOKEN_ARRAY_START:
			parse_array(lex, sem);
			break;
		default:
			parse_scalar(lex, sem);		 
	}

	lex_expect(JSON_PARSE_END, lex, JSON_TOKEN_END);

}
