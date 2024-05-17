elements_object_start(void *state)
{
	ElementsState *_state = (ElementsState *) state;

	 
	if (_state->lex->lex_level == 0)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("cannot call %s on a non-array",
						_state->function_name)));
}
