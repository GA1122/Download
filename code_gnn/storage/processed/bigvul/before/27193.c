static int get_open_max(void)
{
  int open_max = -1;
   
#ifdef F_GETHFDO
  if ((open_max = fcntl(-1, F_GETHFDO, 0)) >= 0)
	return open_max + 1;
#endif
   
  struct rlimit ru;
  if (getrlimit(RLIMIT_NOFILE, &ru) == 0)
	return ru.rlim_cur;
  if ((open_max = sysconf(_SC_OPEN_MAX)) >= 0)
	return open_max;
   
  return 256;
}
