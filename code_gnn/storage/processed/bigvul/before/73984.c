parse_args(int argc, char *argv[])
{
	int r;

	static struct option longOptions[] =
	{
		{"receive-cmd", required_argument, 0, 'v'},
		{"send-cmd", required_argument, 0, 's'},
        {"imap", required_argument, 0, 'I' },
        {"omap", required_argument, 0, 'O' },
        {"emap", required_argument, 0, 'E' },
		{"escape", required_argument, 0, 'e'},
		{"echo", no_argument, 0, 'c'},
		{"noinit", no_argument, 0, 'i'},
		{"noreset", no_argument, 0, 'r'},
		{"nolock", no_argument, 0, 'l'},
		{"flow", required_argument, 0, 'f'},
		{"baud", required_argument, 0, 'b'},
		{"parity", required_argument, 0, 'p'},
		{"databits", required_argument, 0, 'd'},
		{"help", no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};

	r = 0;
	while (1) {
		int optionIndex = 0;
		int c;
		int map;

		 
		opterr = 0;

		c = getopt_long(argc, argv, "hirlcv:s:r:e:f:b:p:d:",
						longOptions, &optionIndex);

		if (c < 0)
			break;

		switch (c) {
		case 's':
			strncpy(opts.send_cmd, optarg, sizeof(opts.send_cmd));
			opts.send_cmd[sizeof(opts.send_cmd) - 1] = '\0';
			break;
		case 'v':
			strncpy(opts.receive_cmd, optarg, sizeof(opts.receive_cmd));
			opts.receive_cmd[sizeof(opts.receive_cmd) - 1] = '\0';
			break;
		case 'I':
			map = parse_map(optarg);
			if (map >= 0) opts.imap = map;
			else { fprintf(stderr, "Invalid --imap\n"); r = -1; }
			break;
		case 'O':
			map = parse_map(optarg);
			if (map >= 0) opts.omap = map;
			else { fprintf(stderr, "Invalid --omap\n"); r = -1; }
			break;
		case 'E':
			map = parse_map(optarg);
			if (map >= 0) opts.emap = map;
			else { fprintf(stderr, "Invalid --emap\n"); r = -1; }
			break;
		case 'c':
			opts.lecho = 1;
			break;
		case 'i':
			opts.noinit = 1;
			break;
		case 'r':
			opts.noreset = 1;
			break;
		case 'l':
#if defined (UUCP_LOCK_DIR) || defined (USE_FLOCK)
			opts.nolock = 1;
#endif
			break;
		case 'e':
			opts.escape = optarg[0] & 0x1f;
			break;
		case 'f':
			switch (optarg[0]) {
			case 'X':
			case 'x':
				opts.flow = FC_XONXOFF;
				break;
			case 'H':
			case 'h':
				opts.flow = FC_RTSCTS;
				break;
			case 'N':
			case 'n':
				opts.flow = FC_NONE;
				break;
			default:
				fprintf(stderr, "Invalid --flow: %c\n", optarg[0]);
				r = -1;
				break;
			}
			break;
		case 'b':
			opts.baud = atoi(optarg);
			break;
		case 'p':
			switch (optarg[0]) {
			case 'e':
				opts.parity = P_EVEN;
				break;
			case 'o':
				opts.parity = P_ODD;
				break;
			case 'n':
				opts.parity = P_NONE;
				break;
			default:
				fprintf(stderr, "Invalid --parity: %c\n", optarg[0]);
				r = -1;
				break;
			}
			break;
		case 'd':
			switch (optarg[0]) {
			case '5':
				opts.databits = 5;
				break;
			case '6':
				opts.databits = 6;
				break;
			case '7':
				opts.databits = 7;
				break;
			case '8':
				opts.databits = 8;
				break;
			default:
				fprintf(stderr, "Invalid --databits: %c\n", optarg[0]);
				r = -1;
				break;
			}
			break;
		case 'h':
			show_usage(argv[0]);
			exit(EXIT_SUCCESS);
		case '?':
		default:
			fprintf(stderr, "Unrecognized option(s)\n");
			r = -1;
			break;
		}
		if ( r < 0 ) {
			fprintf(stderr, "Run with '--help'.\n");
			exit(EXIT_FAILURE);
		}
	}  

	if ( (argc - optind) < 1) {
		fprintf(stderr, "No port given\n");
		fprintf(stderr, "Run with '--help'.\n");
		exit(EXIT_FAILURE);
	}
	strncpy(opts.port, argv[optind], sizeof(opts.port) - 1);
	opts.port[sizeof(opts.port) - 1] = '\0';

	printf("picocom v%s\n", VERSION_STR);
	printf("\n");
	printf("port is        : %s\n", opts.port);
	printf("flowcontrol    : %s\n", flow_str[opts.flow]);
	printf("baudrate is    : %d\n", opts.baud);
	printf("parity is      : %s\n", parity_str[opts.parity]);
	printf("databits are   : %d\n", opts.databits);
	printf("escape is      : C-%c\n", 'a' + opts.escape - 1);
	printf("local echo is  : %s\n", opts.lecho ? "yes" : "no");
	printf("noinit is      : %s\n", opts.noinit ? "yes" : "no");
	printf("noreset is     : %s\n", opts.noreset ? "yes" : "no");
#if defined (UUCP_LOCK_DIR) || defined (USE_FLOCK)
	printf("nolock is      : %s\n", opts.nolock ? "yes" : "no");
#endif
	printf("send_cmd is    : %s\n", 
		   (opts.send_cmd[0] == '\0') ? "disabled" : opts.send_cmd);
	printf("receive_cmd is : %s\n", 
		   (opts.receive_cmd[0] == '\0') ? "disabled" : opts.receive_cmd);
	printf("imap is        : "); print_map(opts.imap);
	printf("omap is        : "); print_map(opts.omap);
	printf("emap is        : "); print_map(opts.emap);
	printf("\n");
}