int main(int argc, uschar **argv)
{
int i;
uschar buffer[1024];

debug_selector = D_v;
debug_file = stderr;
debug_fd = fileno(debug_file);
big_buffer = malloc(big_buffer_size);

for (i = 1; i < argc; i++)
  {
  if (argv[i][0] == '+')
    {
    debug_trace_memory = 2;
    argv[i]++;
    }
  if (isdigit(argv[i][0]))
    debug_selector = Ustrtol(argv[i], NULL, 0);
  else
    if (Ustrspn(argv[i], "abcdefghijklmnopqrtsuvwxyz0123456789-.:/") ==
        Ustrlen(argv[i]))
      {
      #ifdef LOOKUP_LDAP
      eldap_default_servers = argv[i];
      #endif
      #ifdef LOOKUP_MYSQL
      mysql_servers = argv[i];
      #endif
      #ifdef LOOKUP_PGSQL
      pgsql_servers = argv[i];
      #endif
      #ifdef EXPERIMENTAL_REDIS
      redis_servers = argv[i];
      #endif
      }
  #ifdef EXIM_PERL
  else opt_perl_startup = argv[i];
  #endif
  }

printf("Testing string expansion: debug_level = %d\n\n", debug_level);

expand_nstring[1] = US"string 1....";
expand_nlength[1] = 8;
expand_nmax = 1;

#ifdef EXIM_PERL
if (opt_perl_startup != NULL)
  {
  uschar *errstr;
  printf("Starting Perl interpreter\n");
  errstr = init_perl(opt_perl_startup);
  if (errstr != NULL)
    {
    printf("** error in perl_startup code: %s\n", errstr);
    return EXIT_FAILURE;
    }
  }
#endif  

while (fgets(buffer, sizeof(buffer), stdin) != NULL)
  {
  void *reset_point = store_get(0);
  uschar *yield = expand_string(buffer);
  if (yield != NULL)
    {
    printf("%s\n", yield);
    store_reset(reset_point);
    }
  else
    {
    if (search_find_defer) printf("search_find deferred\n");
    printf("Failed: %s\n", expand_string_message);
    if (expand_string_forcedfail) printf("Forced failure\n");
    printf("\n");
    }
  }

search_tidyup();

return 0;
}
