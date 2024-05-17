static void print_usage(const char *program_name) {
  printf("Usage: %s [options] [test name]\n", program_name);
  printf("\n");
  printf("Options:\n");
  printf("  %-20sdisplay this help text.\n", "--help");
  printf("  %-20sdo not run sanity suite.\n", "--insanity");
  printf("\n");
  printf("Valid test names are:\n");
 for (size_t i = 0; i < sanity_suite_size; ++i) {
    printf("  %s\n", sanity_suite[i].function_name);
 }
 for (size_t i = 0; i < test_suite_size; ++i) {
    printf("  %s\n", test_suite[i].function_name);
 }
}
