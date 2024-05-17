acquire_privs (void)
{
  uid_t euid, new_fsuid;

  euid = geteuid ();

   
  if (real_uid != euid)
    {
      if (euid == 0)
        is_privileged = TRUE;
      else
        die ("Unexpected setuid user %d, should be 0", euid);

       
      if (setfsuid (real_uid) < 0)
        die_with_error ("Unable to set fsuid");

       
      new_fsuid = setfsuid (-1);
      if (new_fsuid != real_uid)
        die ("Unable to set fsuid (was %d)", (int)new_fsuid);

       
      set_required_caps ();
    }
  else if (real_uid != 0 && has_caps ())
    {
       
      die ("Unexpected capabilities but not setuid, old file caps config?");
    }

   
}
