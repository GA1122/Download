vaguely_random_number(int max)
{
#ifdef SUPPORT_TLS
# undef vaguely_random_number
#endif
  static pid_t pid = 0;
  pid_t p2;
#if defined(HAVE_SRANDOM) && !defined(HAVE_SRANDOMDEV)
  struct timeval tv;
#endif

  p2 = getpid();
  if (p2 != pid)
    {
    if (pid != 0)
      {

#ifdef HAVE_ARC4RANDOM
       
      arc4random_stir();
#elif defined(HAVE_SRANDOM) || defined(HAVE_SRANDOMDEV)
#ifdef HAVE_SRANDOMDEV
       
      srandomdev();
#else
      gettimeofday(&tv, NULL);
      srandom(tv.tv_sec | tv.tv_usec | getpid());
#endif
#else
       
#endif

      }
    pid = p2;
    }

#ifdef HAVE_ARC4RANDOM
  return arc4random() % max;
#elif defined(HAVE_SRANDOM) || defined(HAVE_SRANDOMDEV)
  return random() % max;
#else
   
  return random_number(max);
#endif
}
