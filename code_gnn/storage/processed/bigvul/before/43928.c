set_fflags(struct archive_write_disk *a)
{
	struct fixup_entry *le;
	unsigned long	set, clear;
	int		r;
	int		critical_flags;
	mode_t		mode = archive_entry_mode(a->entry);

	 
	 
	critical_flags = 0;
#ifdef SF_IMMUTABLE
	critical_flags |= SF_IMMUTABLE;
#endif
#ifdef UF_IMMUTABLE
	critical_flags |= UF_IMMUTABLE;
#endif
#ifdef SF_APPEND
	critical_flags |= SF_APPEND;
#endif
#ifdef UF_APPEND
	critical_flags |= UF_APPEND;
#endif
#ifdef EXT2_APPEND_FL
	critical_flags |= EXT2_APPEND_FL;
#endif
#ifdef EXT2_IMMUTABLE_FL
	critical_flags |= EXT2_IMMUTABLE_FL;
#endif

	if (a->todo & TODO_FFLAGS) {
		archive_entry_fflags(a->entry, &set, &clear);

		 
		if ((critical_flags != 0)  &&  (set & critical_flags)) {
			le = current_fixup(a, a->name);
			if (le == NULL)
				return (ARCHIVE_FATAL);
			le->fixup |= TODO_FFLAGS;
			le->fflags_set = set;
			 
			if ((le->fixup & TODO_MODE) == 0)
				le->mode = mode;
		} else {
			r = set_fflags_platform(a, a->fd,
			    a->name, mode, set, clear);
			if (r != ARCHIVE_OK)
				return (r);
		}
	}
	return (ARCHIVE_OK);
}
