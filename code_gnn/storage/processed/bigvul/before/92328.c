gather_time_entropy(void)
{
#ifdef _WIN32
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);  
  return ft.dwHighDateTime ^ ft.dwLowDateTime;
#else
  struct timeval tv;
  int gettimeofday_res;

  gettimeofday_res = gettimeofday(&tv, NULL);

#if defined(NDEBUG)
  (void)gettimeofday_res;
#else
  assert (gettimeofday_res == 0);
#endif   

   
  return tv.tv_usec;
#endif
}
