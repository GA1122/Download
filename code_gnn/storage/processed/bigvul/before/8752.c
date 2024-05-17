static void log_option(const char *pfx, const uint8_t *opt)
{
	if (dhcp_verbose >= 2) {
		char buf[256 * 2 + 2];
		*bin2hex(buf, (void*) (opt + OPT_DATA), opt[OPT_LEN]) = '\0';
		bb_error_msg("%s: 0x%02x %s", pfx, opt[OPT_CODE], buf);
	}
}
