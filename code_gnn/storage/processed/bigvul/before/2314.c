_PUBLIC_ size_t strlen_m(const char *s)
{
	return strlen_m_ext(s, CH_UNIX, CH_UTF16LE);
}
