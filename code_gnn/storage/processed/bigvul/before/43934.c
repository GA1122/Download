set_mode(struct archive_write_disk *a, int mode)
{
	int r = ARCHIVE_OK;
	mode &= 07777;  

	if (a->todo & TODO_SGID_CHECK) {
		 
		if ((r = lazy_stat(a)) != ARCHIVE_OK)
			return (r);
		if (a->pst->st_gid != a->gid) {
			mode &= ~ S_ISGID;
			if (a->flags & ARCHIVE_EXTRACT_OWNER) {
				 
				archive_set_error(&a->archive, -1,
				    "Can't restore SGID bit");
				r = ARCHIVE_WARN;
			}
		}
		 
		if (a->pst->st_uid != a->uid
		    && (a->todo & TODO_SUID)) {
			mode &= ~ S_ISUID;
			if (a->flags & ARCHIVE_EXTRACT_OWNER) {
				archive_set_error(&a->archive, -1,
				    "Can't restore SUID bit");
				r = ARCHIVE_WARN;
			}
		}
		a->todo &= ~TODO_SGID_CHECK;
		a->todo &= ~TODO_SUID_CHECK;
	} else if (a->todo & TODO_SUID_CHECK) {
		 
		if (a->user_uid != a->uid) {
			mode &= ~ S_ISUID;
			if (a->flags & ARCHIVE_EXTRACT_OWNER) {
				archive_set_error(&a->archive, -1,
				    "Can't make file SUID");
				r = ARCHIVE_WARN;
			}
		}
		a->todo &= ~TODO_SUID_CHECK;
	}

	if (S_ISLNK(a->mode)) {
#ifdef HAVE_LCHMOD
		 
		if (lchmod(a->name, mode) != 0) {
			switch (errno) {
			case ENOTSUP:
			case ENOSYS:
#if ENOTSUP != EOPNOTSUPP
			case EOPNOTSUPP:
#endif
				 
				break;
			default:
				archive_set_error(&a->archive, errno,
				    "Can't set permissions to 0%o", (int)mode);
				r = ARCHIVE_WARN;
			}
		}
#endif
	} else if (!S_ISDIR(a->mode)) {
		 
#ifdef HAVE_FCHMOD
		if (a->fd >= 0) {
			if (fchmod(a->fd, mode) != 0) {
				archive_set_error(&a->archive, errno,
				    "Can't set permissions to 0%o", (int)mode);
				r = ARCHIVE_WARN;
			}
		} else
#endif
			 
			if (chmod(a->name, mode) != 0) {
				archive_set_error(&a->archive, errno,
				    "Can't set permissions to 0%o", (int)mode);
				r = ARCHIVE_WARN;
			}
	}
	return (r);
}
