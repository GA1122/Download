print_dev_fd_list ()
{
  register int i;

  fprintf (stderr, "pid %ld: dev_fd_list:", (long)getpid ());
  fflush (stderr);

  for (i = 0; i < totfds; i++)
    {
      if (dev_fd_list[i])
	fprintf (stderr, " %d", i);
    }
  fprintf (stderr, "\n");
}
