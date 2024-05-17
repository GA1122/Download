static void free_fixed_hostname(struct hostname *host)
{
#if defined(USE_LIBIDN2)
  if(host->encalloc) {
    idn2_free(host->encalloc);  
    host->encalloc = NULL;
  }
#elif defined(USE_WIN32_IDN)
  free(host->encalloc);  
  host->encalloc = NULL;
#else
  (void)host;
#endif
}
