makeJsonLexContextCstringLen(char *json, int len, bool need_escapes)
{
	JsonLexContext *lex = palloc0(sizeof(JsonLexContext));

	lex->input = lex->token_terminator = lex->line_start = json;
	lex->line_number = 1;
	lex->input_length = len;
	if (need_escapes)
		lex->strval = makeStringInfo();
	return lex;
}
