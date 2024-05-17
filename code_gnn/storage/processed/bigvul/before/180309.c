 create_filesystem_object(struct archive_write_disk *a)
 {
 	 
  	const char *linkname;
  	mode_t final_mode, mode;
  	int r;
// 	 
// 	char *linkname_copy;	 
// 	struct archive_string error_string;
// 	int error_number;
  
  	 
  	 
 	linkname = archive_entry_hardlink(a->entry);
 	if (linkname != NULL) {
  #if !HAVE_LINK
  		return (EPERM);
  #else
// 		archive_string_init(&error_string);
// 		linkname_copy = strdup(linkname);
// 		if (linkname_copy == NULL) {
// 		    return (EPERM);
// 		}
// 		 
// 		r = cleanup_pathname_fsobj(linkname_copy, &error_number, &error_string, a->flags);
// 		if (r != ARCHIVE_OK) {
// 			archive_set_error(&a->archive, error_number, "%s", error_string.s);
// 			free(linkname_copy);
// 			 
// 			return (EPERM);
// 		}
// 		r = check_symlinks_fsobj(linkname_copy, &error_number, &error_string, a->flags);
// 		if (r != ARCHIVE_OK) {
// 			archive_set_error(&a->archive, error_number, "%s", error_string.s);
// 			free(linkname_copy);
// 			 
// 			return (EPERM);
// 		}
// 		free(linkname_copy);
  		r = link(linkname, a->name) ? errno : 0;
  		 
 		if (r == 0 && a->filesize <= 0) {
 			a->todo = 0;
 			a->deferred = 0;
 		} else if (r == 0 && a->filesize > 0) {
 			a->fd = open(a->name,
 				     O_WRONLY | O_TRUNC | O_BINARY | O_CLOEXEC);
 			__archive_ensure_cloexec_flag(a->fd);
 			if (a->fd < 0)
 				r = errno;
 		}
 		return (r);
 #endif
 	}
 	linkname = archive_entry_symlink(a->entry);
 	if (linkname != NULL) {
 #if HAVE_SYMLINK
 		return symlink(linkname, a->name) ? errno : 0;
 #else
 		return (EPERM);
 #endif
 	}
 
 	 
 
 	 
 	final_mode = a->mode & 07777;
 	 
 	mode = final_mode & 0777 & ~a->user_umask;
 
 	switch (a->mode & AE_IFMT) {
 	default:
 		 
 		 
 	case AE_IFREG:
 		a->fd = open(a->name,
 		    O_WRONLY | O_CREAT | O_EXCL | O_BINARY | O_CLOEXEC, mode);
 		__archive_ensure_cloexec_flag(a->fd);
 		r = (a->fd < 0);
 		break;
 	case AE_IFCHR:
 #ifdef HAVE_MKNOD
 		 
 		r = mknod(a->name, mode | S_IFCHR,
 		    archive_entry_rdev(a->entry));
 		break;
 #else
 		 
 		return (EINVAL);
 #endif  
 	case AE_IFBLK:
 #ifdef HAVE_MKNOD
 		r = mknod(a->name, mode | S_IFBLK,
 		    archive_entry_rdev(a->entry));
 		break;
 #else
 		 
 		return (EINVAL);
 #endif  
 	case AE_IFDIR:
 		mode = (mode | MINIMUM_DIR_MODE) & MAXIMUM_DIR_MODE;
 		r = mkdir(a->name, mode);
 		if (r == 0) {
 			 
 			a->deferred |= (a->todo & TODO_TIMES);
 			a->todo &= ~TODO_TIMES;
 			 
 			 
 			if ((mode != final_mode)
 			    || (a->flags & ARCHIVE_EXTRACT_PERM))
 				a->deferred |= (a->todo & TODO_MODE);
 			a->todo &= ~TODO_MODE;
 		}
 		break;
 	case AE_IFIFO:
 #ifdef HAVE_MKFIFO
 		r = mkfifo(a->name, mode);
 		break;
 #else
 		 
 		return (EINVAL);
 #endif  
 	}
 
 	 
 	if (r)
 		return (errno);
 
 	 
 	if (mode == final_mode)
 		a->todo &= ~TODO_MODE;
 	return (0);
 }