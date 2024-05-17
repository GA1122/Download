extr_init (void)
{
  we_are_root = geteuid () == ROOT_UID;
  same_permissions_option += we_are_root;
  same_owner_option += we_are_root;

   

  newdir_umask = umask (0);
  if (0 < same_permissions_option)
    current_umask = 0;
  else
    {
      umask (newdir_umask);	 
      current_umask = newdir_umask;
    }
}
