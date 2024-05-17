_PUBLIC_ size_t strlen_m_ext(const char *s, charset_t src_charset, charset_t dst_charset)
{
	struct smb_iconv_handle *ic = get_iconv_handle();
	return strlen_m_ext_handle(ic, s, src_charset, dst_charset);
}
