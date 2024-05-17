parse_file(struct archive_read *a, struct archive_entry *entry,
    struct mtree *mtree, struct mtree_entry *mentry, int *use_next)
{
	const char *path;
	struct stat st_storage, *st;
	struct mtree_entry *mp;
	struct archive_entry *sparse_entry;
	int r = ARCHIVE_OK, r1, parsed_kws;

	mentry->used = 1;

	 
	archive_entry_set_filetype(entry, AE_IFREG);
	archive_entry_set_size(entry, 0);
	archive_string_empty(&mtree->contents_name);

	 
	parsed_kws = 0;
	r = parse_line(a, entry, mtree, mentry, &parsed_kws);

	if (mentry->full) {
		archive_entry_copy_pathname(entry, mentry->name);
		 
		for (mp = mentry->next; mp != NULL; mp = mp->next) {
			if (mp->full && !mp->used
			    && strcmp(mentry->name, mp->name) == 0) {
				 
				mp->used = 1;
				r1 = parse_line(a, entry, mtree, mp,
				    &parsed_kws);
				if (r1 < r)
					r = r1;
			}
		}
	} else {
		 
		size_t n = archive_strlen(&mtree->current_dir);
		if (n > 0)
			archive_strcat(&mtree->current_dir, "/");
		archive_strcat(&mtree->current_dir, mentry->name);
		archive_entry_copy_pathname(entry, mtree->current_dir.s);
		if (archive_entry_filetype(entry) != AE_IFDIR)
			mtree->current_dir.length = n;
	}

	if (mtree->checkfs) {
		 
		mtree->fd = -1;
		if (archive_strlen(&mtree->contents_name) > 0)
			path = mtree->contents_name.s;
		else
			path = archive_entry_pathname(entry);

		if (archive_entry_filetype(entry) == AE_IFREG ||
				archive_entry_filetype(entry) == AE_IFDIR) {
			mtree->fd = open(path, O_RDONLY | O_BINARY | O_CLOEXEC);
			__archive_ensure_cloexec_flag(mtree->fd);
			if (mtree->fd == -1 &&
				(errno != ENOENT ||
				 archive_strlen(&mtree->contents_name) > 0)) {
				archive_set_error(&a->archive, errno,
						"Can't open %s", path);
				r = ARCHIVE_WARN;
			}
		}

		st = &st_storage;
		if (mtree->fd >= 0) {
			if (fstat(mtree->fd, st) == -1) {
				archive_set_error(&a->archive, errno,
						"Could not fstat %s", path);
				r = ARCHIVE_WARN;
				 
				close(mtree->fd);
				mtree->fd = -1;
				st = NULL;
			}
		} else if (lstat(path, st) == -1) {
			st = NULL;
		}

		 
		if (st != NULL) {
			if (((st->st_mode & S_IFMT) == S_IFREG &&
			      archive_entry_filetype(entry) == AE_IFREG)
#ifdef S_IFLNK
			  ||((st->st_mode & S_IFMT) == S_IFLNK &&
			      archive_entry_filetype(entry) == AE_IFLNK)
#endif
#ifdef S_IFSOCK
			  ||((st->st_mode & S_IFSOCK) == S_IFSOCK &&
			      archive_entry_filetype(entry) == AE_IFSOCK)
#endif
#ifdef S_IFCHR
			  ||((st->st_mode & S_IFMT) == S_IFCHR &&
			      archive_entry_filetype(entry) == AE_IFCHR)
#endif
#ifdef S_IFBLK
			  ||((st->st_mode & S_IFMT) == S_IFBLK &&
			      archive_entry_filetype(entry) == AE_IFBLK)
#endif
			  ||((st->st_mode & S_IFMT) == S_IFDIR &&
			      archive_entry_filetype(entry) == AE_IFDIR)
#ifdef S_IFIFO
			  ||((st->st_mode & S_IFMT) == S_IFIFO &&
			      archive_entry_filetype(entry) == AE_IFIFO)
#endif
			) {
				 
			} else {
				 
				if (mtree->fd >= 0)
					close(mtree->fd);
				mtree->fd = -1;
				if (parsed_kws & MTREE_HAS_OPTIONAL) {
					 
					*use_next = 1;
				} else if (r == ARCHIVE_OK) {
					archive_set_error(&a->archive,
					    ARCHIVE_ERRNO_MISC,
					    "mtree specification has different"
					    " type for %s",
					    archive_entry_pathname(entry));
					r = ARCHIVE_WARN;
				}
				return (r);
			}
		}

		 
		if (st != NULL) {
			if (((parsed_kws & MTREE_HAS_DEVICE) == 0 ||
				(parsed_kws & MTREE_HAS_NOCHANGE) != 0) &&
				(archive_entry_filetype(entry) == AE_IFCHR ||
				 archive_entry_filetype(entry) == AE_IFBLK))
				archive_entry_set_rdev(entry, st->st_rdev);
			if ((parsed_kws & (MTREE_HAS_GID | MTREE_HAS_GNAME))
				== 0 ||
			    (parsed_kws & MTREE_HAS_NOCHANGE) != 0)
				archive_entry_set_gid(entry, st->st_gid);
			if ((parsed_kws & (MTREE_HAS_UID | MTREE_HAS_UNAME))
				== 0 ||
			    (parsed_kws & MTREE_HAS_NOCHANGE) != 0)
				archive_entry_set_uid(entry, st->st_uid);
			if ((parsed_kws & MTREE_HAS_MTIME) == 0 ||
			    (parsed_kws & MTREE_HAS_NOCHANGE) != 0) {
#if HAVE_STRUCT_STAT_ST_MTIMESPEC_TV_NSEC
				archive_entry_set_mtime(entry, st->st_mtime,
						st->st_mtimespec.tv_nsec);
#elif HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC
				archive_entry_set_mtime(entry, st->st_mtime,
						st->st_mtim.tv_nsec);
#elif HAVE_STRUCT_STAT_ST_MTIME_N
				archive_entry_set_mtime(entry, st->st_mtime,
						st->st_mtime_n);
#elif HAVE_STRUCT_STAT_ST_UMTIME
				archive_entry_set_mtime(entry, st->st_mtime,
						st->st_umtime*1000);
#elif HAVE_STRUCT_STAT_ST_MTIME_USEC
				archive_entry_set_mtime(entry, st->st_mtime,
						st->st_mtime_usec*1000);
#else
				archive_entry_set_mtime(entry, st->st_mtime, 0);
#endif
			}
			if ((parsed_kws & MTREE_HAS_NLINK) == 0 ||
			    (parsed_kws & MTREE_HAS_NOCHANGE) != 0)
				archive_entry_set_nlink(entry, st->st_nlink);
			if ((parsed_kws & MTREE_HAS_PERM) == 0 ||
			    (parsed_kws & MTREE_HAS_NOCHANGE) != 0)
				archive_entry_set_perm(entry, st->st_mode);
			if ((parsed_kws & MTREE_HAS_SIZE) == 0 ||
			    (parsed_kws & MTREE_HAS_NOCHANGE) != 0)
				archive_entry_set_size(entry, st->st_size);
			archive_entry_set_ino(entry, st->st_ino);
			archive_entry_set_dev(entry, st->st_dev);

			archive_entry_linkify(mtree->resolver, &entry,
				&sparse_entry);
		} else if (parsed_kws & MTREE_HAS_OPTIONAL) {
			 
			*use_next = 1;
			return ARCHIVE_OK;
		}
	}

	mtree->cur_size = archive_entry_size(entry);
	mtree->offset = 0;

	return r;
}
