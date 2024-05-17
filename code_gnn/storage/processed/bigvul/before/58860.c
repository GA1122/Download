exim_setugid(uid_t uid, gid_t gid, BOOL igflag, uschar *msg)
{
uid_t euid = geteuid();
gid_t egid = getegid();

if (euid == root_uid || euid != uid || egid != gid || igflag)
  {
   

  if (igflag)
    {
    struct passwd *pw = getpwuid(uid);
    if (pw != NULL)
      {
      if (initgroups(pw->pw_name, gid) != 0)
        log_write(0,LOG_MAIN|LOG_PANIC_DIE,"initgroups failed for uid=%ld: %s",
          (long int)uid, strerror(errno));
      }
    else log_write(0, LOG_MAIN|LOG_PANIC_DIE, "cannot run initgroups(): "
      "no passwd entry for uid=%ld", (long int)uid);
    }

  if (setgid(gid) < 0 || setuid(uid) < 0)
    {
    log_write(0, LOG_MAIN|LOG_PANIC_DIE, "unable to set gid=%ld or uid=%ld "
      "(euid=%ld): %s", (long int)gid, (long int)uid, (long int)euid, msg);
    }
  }

 

DEBUG(D_uid)
  {
  int group_count, save_errno;
  gid_t group_list[NGROUPS_MAX];
  debug_printf("changed uid/gid: %s\n  uid=%ld gid=%ld pid=%ld\n", msg,
    (long int)geteuid(), (long int)getegid(), (long int)getpid());
  group_count = getgroups(NGROUPS_MAX, group_list);
  save_errno = errno;
  debug_printf("  auxiliary group list:");
  if (group_count > 0)
    {
    int i;
    for (i = 0; i < group_count; i++) debug_printf(" %d", (int)group_list[i]);
    }
  else if (group_count < 0)
    debug_printf(" <error: %s>", strerror(save_errno));
  else debug_printf(" <none>");
  debug_printf("\n");
  }
}