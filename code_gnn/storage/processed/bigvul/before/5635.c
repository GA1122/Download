ask (char const *format, ...)
{
  static int ttyfd = -2;
  ssize_t r;
  va_list args;

  va_start (args, format);
  vfprintf (stdout, format, args);
  va_end (args);
  fflush (stdout);

  if (ttyfd == -2)
    {
       
      ttyfd = (posixly_correct || isatty (STDOUT_FILENO)
	       ? open (TTY_DEVICE, O_RDONLY)
	       : -1);
    }

  if (ttyfd < 0)
    {
       
      printf ("\n");
      buf[0] = '\n';
      buf[1] = '\0';
    }
  else
    {
      size_t s = 0;
      while ((r = read (ttyfd, buf + s, bufsize - 1 - s)) == bufsize - 1 - s
	     && buf[bufsize - 2] != '\n')
	{
	  s = bufsize - 1;
	  bufsize *= 2;
	  buf = realloc (buf, bufsize);
	  if (!buf)
	    xalloc_die ();
	}
      if (r == 0)
	printf ("EOF\n");
      else if (r < 0)
	{
	  perror ("tty read");
	  fflush (stderr);
	  close (ttyfd);
	  ttyfd = -1;
	  r = 0;
	}
      buf[s + r] = '\0';
    }
}
