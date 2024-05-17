elements_array_element_start(void *state, bool isnull)
{
	ElementsState *_state = (ElementsState *) state;

	 
	if (_state->lex->lex_level == 1)
	{
		 
		if (_state->normalize_results && _state->lex->token_type == JSON_TOKEN_STRING)
			_state->next_scalar = true;
		else
			_state->result_start = _state->lex->token_start;
	}
}
