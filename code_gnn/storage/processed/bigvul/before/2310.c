_PUBLIC_ bool strhaslower(const char *string)
{
	struct smb_iconv_handle *ic = get_iconv_handle();
	return strhaslower_handle(ic, string);
}
