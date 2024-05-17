_dbus_check_setuid (void)
{
   
#if 0 && defined(HAVE_LIBC_ENABLE_SECURE)
  {
     
    extern int __libc_enable_secure;
    return __libc_enable_secure;
  }
#elif defined(HAVE_ISSETUGID)
   
  return issetugid ();
#else
  uid_t ruid, euid, suid;  
  gid_t rgid, egid, sgid;  

  static dbus_bool_t check_setuid_initialised;
  static dbus_bool_t is_setuid;

  if (_DBUS_UNLIKELY (!check_setuid_initialised))
    {
#ifdef HAVE_GETRESUID
      if (getresuid (&ruid, &euid, &suid) != 0 ||
          getresgid (&rgid, &egid, &sgid) != 0)
#endif  
        {
          suid = ruid = getuid ();
          sgid = rgid = getgid ();
          euid = geteuid ();
          egid = getegid ();
        }

      check_setuid_initialised = TRUE;
      is_setuid = (ruid != euid || ruid != suid ||
                   rgid != egid || rgid != sgid);

    }
  return is_setuid;
#endif
}
