_PUBLIC_ size_t strlen_m_ext_term_null(const char *s,
				       const charset_t src_charset,
				       const charset_t dst_charset)
{
	size_t len;
	if (!s) {
		return 0;
	}
	len = strlen_m_ext(s, src_charset, dst_charset);
	if (len == 0) {
		return 0;
	}

	return len+1;
}
