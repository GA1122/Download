_PUBLIC_ bool strhasupper(const char *string)
{
	struct smb_iconv_handle *ic = get_iconv_handle();
	return strhasupper_handle(ic, string);
}
