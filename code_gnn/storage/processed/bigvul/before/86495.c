current_getpwuid(void)
{
  uid_t ruid;

   
  errno = 0;
  ruid = getuid ();

  return errno == 0 ? getpwuid (ruid) : NULL;
}
