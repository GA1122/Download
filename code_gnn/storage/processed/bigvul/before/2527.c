json_build_array_noargs(PG_FUNCTION_ARGS)
{
	PG_RETURN_TEXT_P(cstring_to_text_with_len("[]", 2));
}
