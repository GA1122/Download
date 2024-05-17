int main(int argc, char *argv[])
{
	int c;
	g_options.log_destination = LOGGING_STDERR;
	g_options.only_desired_port = 1;

	while ((c = getopt(argc, argv, "qnhdp:P:s:lv:m:N")) != -1) {
		switch (c) {
		case '?':
		case 'h':
			g_options.help_mode = 1;
			break;
		case 'p':
		case 'P':
		{
			long long port = 0;
			port = atoi(optarg);
			if (port < 0) {
				ERR("Port number must be non-negative");
				return 1;
			}
			if (port > UINT16_MAX) {
				ERR("Port number must be %u or less, "
				    "but not negative", UINT16_MAX);
				return 2;
			}
			g_options.desired_port = (uint16_t)port;
			if (c == 'p')
			  g_options.only_desired_port = 1;
			else
			  g_options.only_desired_port = 0;
			break;
		}
		case 'l':
			g_options.log_destination = LOGGING_SYSLOG;
			break;
		case 'd':
			g_options.nofork_mode = 1;
			g_options.verbose_mode = 1;
			break;
		case 'q':
			g_options.verbose_mode = 1;
			break;
		case 'n':
			g_options.nofork_mode = 1;
			break;
		case 'v':
			g_options.vendor_id = strto16(optarg);
			break;
		case 'm':
			g_options.product_id = strto16(optarg);
			break;
		case 's':
			g_options.serial_num = (unsigned char *)optarg;
			break;
		case 'N':
			g_options.noprinter_mode = 1;
			break;
		}
	}

	if (g_options.help_mode) {
		printf(
		"Usage: %s -v <vendorid> -m <productid> -p <port>\n"
		"Options:\n"
		"  -h           Show this help message\n"
		"  -v <vid>     Vendor ID of desired printer\n"
		"  -m <pid>     Product ID of desired printer\n"
		"  -s <serial>  Serial number of desired printer\n"
		"  -p <portnum> Port number to bind against, error out if port already taken\n"
		"  -P <portnum> Port number to bind against, use another port if port already\n"
		"               taken\n"
		"  -l           Redirect logging to syslog\n"
		"  -q           Enable verbose tracing\n"
		"  -d           Debug mode for verbose output and no fork\n"
		"  -n           No-fork mode\n"
		"  -N           No-printer mode, debug/developer mode which makes ippusbxd\n"
		"               run without IPP-over-USB printer\n"
		, argv[0]);
		return 0;
	}

	start_daemon();
	return 0;
}