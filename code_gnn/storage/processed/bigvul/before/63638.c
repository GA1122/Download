int g_istr_cmp(gconstpointer v, gconstpointer v2)
{
	return g_ascii_strcasecmp((const char *) v, (const char *) v2);
}