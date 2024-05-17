_PUBLIC_ bool strhaslower_handle(struct smb_iconv_handle *ic,
				 const char *string)
{
	while (*string) {
		size_t c_size;
		codepoint_t s;
		codepoint_t t;

		s = next_codepoint_handle(ic, string, &c_size);
		string += c_size;

		t = toupper_m(s);

		if (s != t) {
			return true;  
		}
	}

	return false;
}
