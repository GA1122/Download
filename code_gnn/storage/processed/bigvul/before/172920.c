static bool parse_args(int argc, char **argv) {
 while (1) {
 int option_index = 0;
 int c = getopt_long_only(argc, argv, "", long_options, &option_index);
 if (c != 0)
 break;

 switch (c) {
 case 0:
 if (option_index == 0) {
 if (!string_to_bdaddr(optarg, &bt_remote_bdaddr)) {
 return false;
 }
 }
 if (option_index == 1) {
          discover = true;
 }
 if (option_index == 2) {
          discoverable = true;
 }
 if (option_index == 3) {
          timeout_in_sec = atoi(optarg);
 }
 if (option_index == 4) {
          bond  = true;
 }
 if (option_index == 5) {
          up = true;
 }
 if (option_index == 6) {
          f_verbose++;
 }
 if (option_index == 7) {
          get_name = true;
 }
 if (option_index == 8) {
          bd_name = (char *)optarg;
          set_name = true;
 }
 if (option_index == 9) {
          sco_listen = true;
 }
 if (option_index == 10) {
          sco_connect = true;
 }
 break;

 default:
        fprintf(stderr, "?? getopt returned character code 0%o ??\n", c);
 }
 }

 if (optind < argc) {
    fprintf(stderr, "non-option ARGV-elements: ");
 while (optind < argc)
      fprintf(stderr, "%s ", argv[optind++]);
    fprintf(stderr, "\n");
 return false;
 }
 return true;
}
