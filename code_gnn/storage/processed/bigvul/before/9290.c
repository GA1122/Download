clear_fifo_list ()
{
  register int i;

  if (nfds == 0)
    return;

  for (i = 0; nfds && i < totfds; i++)
    clear_fifo (i);

  nfds = 0;
}
