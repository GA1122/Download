static UPNP_INLINE int is_valid_alias(
	 
	const struct xml_alias_t *alias)
{
	return alias->doc.buf != NULL;
}
