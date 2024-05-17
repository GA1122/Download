evaluate_uid(void)
{
  uid_t ruid = getuid();
  uid_t euid = geteuid();

   
  return (uid_t) 0 == ruid && ruid == euid ? 0 : 1;
}
