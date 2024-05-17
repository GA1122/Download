_PUBLIC_ codepoint_t next_codepoint(const char *str, size_t *size)
{
	return next_codepoint_handle(get_iconv_handle(), str, size);
}
