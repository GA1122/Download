set_file_attributes (char const *to, enum file_attributes attr,
		     char const *from, struct stat *st, mode_t mode,
		     struct timespec *new_time)
{
  if (attr & FA_TIMES)
    {
      struct timespec times[2];
      if (new_time)
	times[0] = times[1] = *new_time;
      else
        {
	  times[0] = get_stat_atime (st);
	  times[1] = get_stat_mtime (st);
	}
      if (lutimens (to, times) != 0)
	pfatal ("Failed to set the timestamps of %s %s",
		S_ISLNK (st->st_mode) ? "symbolic link" : "file",
		quotearg (to));
    }
  if (attr & FA_IDS)
    {
      static uid_t euid = -1;
      static gid_t egid = -1;
      uid_t uid;
      uid_t gid;

      if (euid == -1)
        {
	  euid = geteuid ();
	  egid = getegid ();
	}
      uid = (euid == st->st_uid) ? -1 : st->st_uid;
      gid = (egid == st->st_gid) ? -1 : st->st_gid;

       
      if ((uid != -1 || gid != -1)
	  && lchown (to, uid, gid) != 0
	  && (errno != EPERM
	      || (uid != -1
		  && lchown (to, (uid = -1), gid) != 0
		  && errno != EPERM)))
	pfatal ("Failed to set the %s of %s %s",
		(uid == -1) ? "owner" : "owning group",
		S_ISLNK (st->st_mode) ? "symbolic link" : "file",
		quotearg (to));
    }
  if (copy_attr (from, to))
    fatal_exit (0);
   
  if (attr & FA_MODE)
    {
#if 0 && defined HAVE_LCHMOD
       
      if (lchmod (to, mode))
#else
      if (! S_ISLNK (mode) && chmod (to, mode) != 0)
#endif
	pfatal ("Failed to set the permissions of %s %s",
		S_ISLNK (st->st_mode) ? "symbolic link" : "file",
		quotearg (to));
    }
}
