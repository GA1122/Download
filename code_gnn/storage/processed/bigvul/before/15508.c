unsigned FAST_FUNC udhcp_option_idx(const char *name)
{
	int n = index_in_strings(dhcp_option_strings, name);
	if (n >= 0)
		return n;

	{
		char buf[sizeof(dhcp_option_strings)];
		char *d = buf;
		const char *s = dhcp_option_strings;
		while (s < dhcp_option_strings + sizeof(dhcp_option_strings) - 2) {
			*d++ = (*s == '\0' ? ' ' : *s);
			s++;
		}
		*d = '\0';
		bb_error_msg_and_die("unknown option '%s', known options: %s", name, buf);
	}
}
