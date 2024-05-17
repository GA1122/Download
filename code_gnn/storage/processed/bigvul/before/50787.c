cleanup_backslash_2(wchar_t *p)
{

	 
	while (*p != L'\0') {
		if (*p == L'\\')
			*p = L'/';
		p++;
	}
}
