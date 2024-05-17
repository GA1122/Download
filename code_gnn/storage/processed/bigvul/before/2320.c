_PUBLIC_ int strncasecmp_m(const char *s1, const char *s2, size_t n)
{
	struct smb_iconv_handle *iconv_handle = get_iconv_handle();
	return strncasecmp_m_handle(iconv_handle, s1, s2, n);
}
