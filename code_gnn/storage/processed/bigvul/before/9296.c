make_named_pipe ()
{
  char *tname;

  tname = sh_mktmpname ("sh-np", MT_USERANDOM|MT_USETMPDIR);
  if (mkfifo (tname, 0600) < 0)
    {
      free (tname);
      return ((char *)NULL);
    }

  add_fifo_list (tname);
  return (tname);
}
