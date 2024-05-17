static void usage() {
  fputs("Usage: -b [<tid>]\n"
 "  -b dump backtrace to console, otherwise dump full tombstone file\n"
 "\n"
 "If tid specified, sends a request to debuggerd to dump that task.\n"
 "Otherwise, starts the debuggerd server.\n", stderr);
}
