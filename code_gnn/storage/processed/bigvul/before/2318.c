_PUBLIC_ size_t strlen_m_term(const char *s)
{
	return strlen_m_ext_term(s, CH_UNIX, CH_UTF16LE);
}
