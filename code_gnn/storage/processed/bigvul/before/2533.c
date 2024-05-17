json_lex_number(JsonLexContext *lex, char *s, bool *num_err)
{
	bool		error = false;
	char	   *p;
	int			len;

	len = s - lex->input;
	 
	 

	 
	if (*s == '0')
	{
		s++;
		len++;
	}
	else if (*s >= '1' && *s <= '9')
	{
		do
		{
			s++;
			len++;
		} while (len < lex->input_length && *s >= '0' && *s <= '9');
	}
	else
		error = true;

	 
	if (len < lex->input_length && *s == '.')
	{
		s++;
		len++;
		if (len == lex->input_length || *s < '0' || *s > '9')
			error = true;
		else
		{
			do
			{
				s++;
				len++;
			} while (len < lex->input_length && *s >= '0' && *s <= '9');
		}
	}

	 
	if (len < lex->input_length && (*s == 'e' || *s == 'E'))
	{
		s++;
		len++;
		if (len < lex->input_length && (*s == '+' || *s == '-'))
		{
			s++;
			len++;
		}
		if (len == lex->input_length || *s < '0' || *s > '9')
			error = true;
		else
		{
			do
			{
				s++;
				len++;
			} while (len < lex->input_length && *s >= '0' && *s <= '9');
		}
	}

	 
	for (p = s; len < lex->input_length && JSON_ALPHANUMERIC_CHAR(*p); p++, len++)
		error = true;

	if (num_err != NULL)
	{
		 
		*num_err = error;
	}
	else
	{
		lex->prev_token_terminator = lex->token_terminator;
		lex->token_terminator = p;
		if (error)
			report_invalid_token(lex);
	}
}
