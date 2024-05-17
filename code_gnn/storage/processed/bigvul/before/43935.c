set_ownership(struct archive_write_disk *a)
{
#ifndef __CYGWIN__
 

	 
	if (a->user_uid != 0  &&  a->user_uid != a->uid) {
		archive_set_error(&a->archive, errno,
		    "Can't set UID=%jd", (intmax_t)a->uid);
		return (ARCHIVE_WARN);
	}
#endif

#ifdef HAVE_FCHOWN
	 
	if (a->fd >= 0 && fchown(a->fd, a->uid, a->gid) == 0) {
		 
		a->todo &= ~(TODO_OWNER | TODO_SGID_CHECK | TODO_SUID_CHECK);
		return (ARCHIVE_OK);
	}
#endif

	 
	 
#ifdef HAVE_LCHOWN
	if (lchown(a->name, a->uid, a->gid) == 0) {
		 
		a->todo &= ~(TODO_OWNER | TODO_SGID_CHECK | TODO_SUID_CHECK);
		return (ARCHIVE_OK);
	}
#elif HAVE_CHOWN
	if (!S_ISLNK(a->mode) && chown(a->name, a->uid, a->gid) == 0) {
		 
		a->todo &= ~(TODO_OWNER | TODO_SGID_CHECK | TODO_SUID_CHECK);
		return (ARCHIVE_OK);
	}
#endif

	archive_set_error(&a->archive, errno,
	    "Can't set user=%jd/group=%jd for %s",
	    (intmax_t)a->uid, (intmax_t)a->gid, a->name);
	return (ARCHIVE_WARN);
}
