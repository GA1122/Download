_archive_write_disk_close(struct archive *_a)
{
	struct archive_write_disk *a = (struct archive_write_disk *)_a;
	struct fixup_entry *next, *p;
	int ret;

	archive_check_magic(&a->archive, ARCHIVE_WRITE_DISK_MAGIC,
	    ARCHIVE_STATE_HEADER | ARCHIVE_STATE_DATA,
	    "archive_write_disk_close");
	ret = _archive_write_disk_finish_entry(&a->archive);

	 
	p = sort_dir_list(a->fixup_list);

	while (p != NULL) {
		a->pst = NULL;  
		if (p->fixup & TODO_TIMES) {
			set_times(a, -1, p->mode, p->name,
			    p->atime, p->atime_nanos,
			    p->birthtime, p->birthtime_nanos,
			    p->mtime, p->mtime_nanos,
			    p->ctime, p->ctime_nanos);
		}
		if (p->fixup & TODO_MODE_BASE)
			chmod(p->name, p->mode);
		if (p->fixup & TODO_ACLS)
			archive_write_disk_set_acls(&a->archive,
						    -1, p->name, &p->acl);
		if (p->fixup & TODO_FFLAGS)
			set_fflags_platform(a, -1, p->name,
			    p->mode, p->fflags_set, 0);
		if (p->fixup & TODO_MAC_METADATA)
			set_mac_metadata(a, p->name, p->mac_metadata,
					 p->mac_metadata_size);
		next = p->next;
		archive_acl_clear(&p->acl);
		free(p->mac_metadata);
		free(p->name);
		free(p);
		p = next;
	}
	a->fixup_list = NULL;
	return (ret);
}