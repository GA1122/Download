option_clear_or_none(const char *o)
{
	return o == NULL || strcasecmp(o, "none") == 0;
}
