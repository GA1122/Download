acquire_privs (void)
{
  uid_t euid, new_fsuid;

  euid = geteuid ();

   
  if (real_uid != euid)
    {
      if (euid != 0)
        die ("Unexpected setuid user %d, should be 0", euid);

      is_privileged = TRUE;
       
      if (setfsuid (real_uid) < 0)
        die_with_error ("Unable to set fsuid");

       
      new_fsuid = setfsuid (-1);
      if (new_fsuid != real_uid)
        die ("Unable to set fsuid (was %d)", (int)new_fsuid);

       
      drop_cap_bounding_set (TRUE);

       
      set_required_caps ();
    }
  else if (real_uid != 0 && has_caps ())
    {
       
      die ("Unexpected capabilities but not setuid, old file caps config?");
    }
  else if (real_uid == 0)
    {
       
      struct __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
      struct __user_cap_data_struct data[2] = { { 0 } };

      if (capget (&hdr, data) < 0)
        die_with_error ("capget (for uid == 0) failed");

      requested_caps[0] = data[0].effective;
      requested_caps[1] = data[1].effective;
    }

   
}
