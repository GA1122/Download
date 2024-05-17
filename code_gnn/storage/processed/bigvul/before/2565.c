alen_array_element_start(void *state, bool isnull)
{
	AlenState  *_state = (AlenState *) state;

	 
	if (_state->lex->lex_level == 1)
		_state->count++;
}
