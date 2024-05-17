_archive_write_disk_finish_entry(struct archive *_a)
{
	struct archive_write_disk *a = (struct archive_write_disk *)_a;
	int ret = ARCHIVE_OK;

	archive_check_magic(&a->archive, ARCHIVE_WRITE_DISK_MAGIC,
	    ARCHIVE_STATE_HEADER | ARCHIVE_STATE_DATA,
	    "archive_write_finish_entry");
	if (a->archive.state & ARCHIVE_STATE_HEADER)
		return (ARCHIVE_OK);
	archive_clear_error(&a->archive);

	 
	if (a->fd < 0) {
		 
	} else if (a->filesize < 0) {
		 
	} else if (a->fd_offset == a->filesize) {
		 
		 
#if defined(__APPLE__) && defined(UF_COMPRESSED) && defined(HAVE_ZLIB_H)
	} else if (a->todo & TODO_HFS_COMPRESSION) {
		char null_d[1024];
		ssize_t r;

		if (a->file_remaining_bytes)
			memset(null_d, 0, sizeof(null_d));
		while (a->file_remaining_bytes) {
			if (a->file_remaining_bytes > sizeof(null_d))
				r = hfs_write_data_block(
				    a, null_d, sizeof(null_d));
			else
				r = hfs_write_data_block(
				    a, null_d, a->file_remaining_bytes);
			if (r < 0)
				return ((int)r);
		}
#endif
	} else {
#if HAVE_FTRUNCATE
		if (ftruncate(a->fd, a->filesize) == -1 &&
		    a->filesize == 0) {
			archive_set_error(&a->archive, errno,
			    "File size could not be restored");
			return (ARCHIVE_FAILED);
		}
#endif
		 
		a->pst = NULL;
		if ((ret = lazy_stat(a)) != ARCHIVE_OK)
			return (ret);
		 
		if (a->st.st_size < a->filesize) {
			const char nul = '\0';
			if (lseek(a->fd, a->filesize - 1, SEEK_SET) < 0) {
				archive_set_error(&a->archive, errno,
				    "Seek failed");
				return (ARCHIVE_FATAL);
			}
			if (write(a->fd, &nul, 1) < 0) {
				archive_set_error(&a->archive, errno,
				    "Write to restore size failed");
				return (ARCHIVE_FATAL);
			}
			a->pst = NULL;
		}
	}

	 

	 
	if (a->todo & TODO_APPLEDOUBLE) {
		int r2 = fixup_appledouble(a, a->name);
		if (r2 == ARCHIVE_EOF) {
			 
			goto finish_metadata;
		}
		if (r2 < ret) ret = r2;
	}

	 
	if (a->todo & (TODO_OWNER | TODO_SUID | TODO_SGID)) {
		a->uid = archive_write_disk_uid(&a->archive,
		    archive_entry_uname(a->entry),
		    archive_entry_uid(a->entry));
	}
	 
	 
	if (a->todo & (TODO_OWNER | TODO_SGID | TODO_SUID)) {
		a->gid = archive_write_disk_gid(&a->archive,
		    archive_entry_gname(a->entry),
		    archive_entry_gid(a->entry));
	 }

	 
	if (a->todo & TODO_OWNER) {
		int r2 = set_ownership(a);
		if (r2 < ret) ret = r2;
	}

	 
	if (a->todo & TODO_MODE) {
		int r2 = set_mode(a, a->mode);
		if (r2 < ret) ret = r2;
	}

	 
	if (a->todo & TODO_XATTR) {
		int r2 = set_xattrs(a);
		if (r2 < ret) ret = r2;
	}

	 
	if (a->todo & TODO_FFLAGS) {
		int r2 = set_fflags(a);
		if (r2 < ret) ret = r2;
	}

	 
	if (a->todo & TODO_TIMES) {
		int r2 = set_times_from_entry(a);
		if (r2 < ret) ret = r2;
	}

	 
	if (a->todo & TODO_MAC_METADATA) {
		const void *metadata;
		size_t metadata_size;
		metadata = archive_entry_mac_metadata(a->entry, &metadata_size);
		if (metadata != NULL && metadata_size > 0) {
			int r2 = set_mac_metadata(a, archive_entry_pathname(
			    a->entry), metadata, metadata_size);
			if (r2 < ret) ret = r2;
		}
	}

	 
	if (a->todo & TODO_ACLS) {
		int r2 = archive_write_disk_set_acls(&a->archive, a->fd,
				  archive_entry_pathname(a->entry),
				  archive_entry_acl(a->entry));
		if (r2 < ret) ret = r2;
	}

finish_metadata:
	 
	if (a->fd >= 0) {
		close(a->fd);
		a->fd = -1;
	}
	 
	if (a->entry) {
		archive_entry_free(a->entry);
		a->entry = NULL;
	}
	a->archive.state = ARCHIVE_STATE_HEADER;
	return (ret);
}
