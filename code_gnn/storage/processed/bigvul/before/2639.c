okeys_object_field_start(void *state, char *fname, bool isnull)
{
	OkeysState *_state = (OkeysState *) state;

	 
	if (_state->lex->lex_level != 1)
		return;

	 
	if (_state->result_count >= _state->result_size)
	{
		_state->result_size *= 2;
		_state->result = (char **)
			repalloc(_state->result, sizeof(char *) * _state->result_size);
	}

	 
	_state->result[_state->result_count++] = pstrdup(fname);
}
