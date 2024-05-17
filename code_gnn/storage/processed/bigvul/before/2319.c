_PUBLIC_ size_t strlen_m_term_null(const char *s)
{
	return strlen_m_ext_term_null(s, CH_UNIX, CH_UTF16LE);
}
