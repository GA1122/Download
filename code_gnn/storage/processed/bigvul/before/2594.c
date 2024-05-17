hash_object_field_end(void *state, char *fname, bool isnull)
{
	JHashState *_state = (JHashState *) state;
	JsonHashEntry *hashentry;
	bool		found;

	 
	if (_state->lex->lex_level > 2)
		return;

	 
	if (strlen(fname) >= NAMEDATALEN)
		return;

	hashentry = hash_search(_state->hash, fname, HASH_ENTER, &found);

	 

	hashentry->isnull = isnull;
	if (_state->save_json_start != NULL)
	{
		int			len = _state->lex->prev_token_terminator - _state->save_json_start;
		char	   *val = palloc((len + 1) * sizeof(char));

		memcpy(val, _state->save_json_start, len);
		val[len] = '\0';
		hashentry->val = val;
	}
	else
	{
		 
		hashentry->val = _state->saved_scalar;
	}
}
