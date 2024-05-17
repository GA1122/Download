restore_entry(struct archive_write_disk *a)
{
	int ret = ARCHIVE_OK, en;

	if (a->flags & ARCHIVE_EXTRACT_UNLINK && !S_ISDIR(a->mode)) {
		 
		if (unlink(a->name) == 0) {
			 
			a->pst = NULL;
		} else if (errno == ENOENT) {
			 
		} else if (rmdir(a->name) == 0) {
			 
			a->pst = NULL;
		} else {
			 
			archive_set_error(&a->archive, errno,
			    "Could not unlink");
			return(ARCHIVE_FAILED);
		}
	}

	 
	en = create_filesystem_object(a);

	if ((en == ENOTDIR || en == ENOENT)
	    && !(a->flags & ARCHIVE_EXTRACT_NO_AUTODIR)) {
		 
		create_parent_dir(a, a->name);
		 
		en = create_filesystem_object(a);
	}

	if ((en == EISDIR || en == EEXIST)
	    && (a->flags & ARCHIVE_EXTRACT_NO_OVERWRITE)) {
		 
		archive_entry_unset_size(a->entry);
		return (ARCHIVE_OK);
	}

	 
	if (en == EISDIR) {
		 
		if (rmdir(a->name) != 0) {
			archive_set_error(&a->archive, errno,
			    "Can't remove already-existing dir");
			return (ARCHIVE_FAILED);
		}
		a->pst = NULL;
		 
		en = create_filesystem_object(a);
	} else if (en == EEXIST) {
		 
		int r = 0;
		 
		if (S_ISDIR(a->mode))
			r = stat(a->name, &a->st);
		 
		if (r != 0 || !S_ISDIR(a->mode))
			r = lstat(a->name, &a->st);
		if (r != 0) {
			archive_set_error(&a->archive, errno,
			    "Can't stat existing object");
			return (ARCHIVE_FAILED);
		}

		 
		if ((a->flags & ARCHIVE_EXTRACT_NO_OVERWRITE_NEWER)
		    &&  !S_ISDIR(a->st.st_mode)) {
			if (!older(&(a->st), a->entry)) {
				archive_entry_unset_size(a->entry);
				return (ARCHIVE_OK);
			}
		}

		 
		if (a->skip_file_set &&
		    a->st.st_dev == (dev_t)a->skip_file_dev &&
		    a->st.st_ino == (ino_t)a->skip_file_ino) {
			archive_set_error(&a->archive, 0,
			    "Refusing to overwrite archive");
			return (ARCHIVE_FAILED);
		}

		if (!S_ISDIR(a->st.st_mode)) {
			 
			if (unlink(a->name) != 0) {
				archive_set_error(&a->archive, errno,
				    "Can't unlink already-existing object");
				return (ARCHIVE_FAILED);
			}
			a->pst = NULL;
			 
			en = create_filesystem_object(a);
		} else if (!S_ISDIR(a->mode)) {
			 
			if (rmdir(a->name) != 0) {
				archive_set_error(&a->archive, errno,
				    "Can't replace existing directory with non-directory");
				return (ARCHIVE_FAILED);
			}
			 
			en = create_filesystem_object(a);
		} else {
			 
			if ((a->mode != a->st.st_mode)
			    && (a->todo & TODO_MODE_FORCE))
				a->deferred |= (a->todo & TODO_MODE);
			 
			en = 0;  
		}
	}

	if (en) {
		 
		archive_set_error(&a->archive, en, "Can't create '%s'",
		    a->name);
		return (ARCHIVE_FAILED);
	}

	a->pst = NULL;  
	return (ret);
}
