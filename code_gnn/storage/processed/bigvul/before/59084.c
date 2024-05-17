comics_regex_quote (const gchar *unquoted_string)
{
	const gchar *p;
	GString *dest;

	dest = g_string_new ("'");

	p = unquoted_string;

	while (*p) {
		switch (*p) {
			 
			case ('*'):
			 
			case ('?'):
			 
			case ('['):
				g_string_append (dest, "[");
				g_string_append_c (dest, *p);
				g_string_append (dest, "]");
				break;
			 
			case ('\\'):
				g_string_append (dest, "[\\\\]");
				break;
			 
			case ('\''):
				g_string_append (dest, "'\\''");
				break;
			default:
				g_string_append_c (dest, *p);
				break;
		}
		++p;
	}
	g_string_append_c (dest, '\'');
	return g_string_free (dest, FALSE);
}
