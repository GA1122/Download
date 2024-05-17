dfawarn (char const *mesg)
{
  static enum { DW_NONE = 0, DW_POSIX, DW_GNU } mode;
  if (mode == DW_NONE)
    mode = (getenv ("POSIXLY_CORRECT") ? DW_POSIX : DW_GNU);
  if (mode == DW_GNU)
    dfaerror (mesg);
}
