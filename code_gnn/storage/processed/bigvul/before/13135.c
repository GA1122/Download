static int FAST_FUNC read_optset(const char *line, void *arg) {
	return udhcp_str2optset(line, arg,
			dhcp_optflags, dhcp_option_strings,
			  0
	);
}
