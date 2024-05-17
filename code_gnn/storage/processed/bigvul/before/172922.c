static void usage(const char *name) {
  fprintf(stderr, "Usage: %s [--bond|--discover|--discoverable|--up|--sco_listen|--sco_connect] [--bdaddr=<bdaddr>] [--time=<time_in_sec>] --verbose\n", name);
  fprintf(stderr, "     bond: Discover actively advertising devices\n");
  fprintf(stderr, "     discover: Discover actively advertising devices\n");
  fprintf(stderr, "     discoverable: Set into a connectable and discoverable mode\n");
  fprintf(stderr, "     up: Only bring up stack\n");
  fprintf(stderr, "     sco_listen: Listen for incoming SCO connections\n");
  fprintf(stderr, "     sco_connect: Establish a SCO connection with another device\n");
  fprintf(stderr, "     time: Time to hold in the specified mode\n");
  exit(1);
}
