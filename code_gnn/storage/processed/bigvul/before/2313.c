_PUBLIC_ bool strhasupper_handle(struct smb_iconv_handle *ic,
				 const char *string)
{
	while (*string) {
		size_t c_size;
		codepoint_t s;
		codepoint_t t;

		s = next_codepoint_handle(ic, string, &c_size);
		string += c_size;

		t = tolower_m(s);

		if (s != t) {
			return true;  
		}
	}

	return false;
}
