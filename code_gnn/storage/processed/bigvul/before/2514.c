IsValidJsonNumber(const char *str, int len)
{
	bool		numeric_error;
	JsonLexContext dummy_lex;


	 
	if (*str == '-')
	{
		dummy_lex.input = (char *) str + 1;
		dummy_lex.input_length = len - 1;
	}
	else
	{
		dummy_lex.input = (char *) str;
		dummy_lex.input_length = len;
	}

	json_lex_number(&dummy_lex, dummy_lex.input, &numeric_error);

	return !numeric_error;
}
