okeys_array_start(void *state)
{
	OkeysState *_state = (OkeysState *) state;

	 
	if (_state->lex->lex_level == 0)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("cannot call %s on an array",
						"json_object_keys")));
}
