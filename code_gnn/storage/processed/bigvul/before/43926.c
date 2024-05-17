older(struct stat *st, struct archive_entry *entry)
{
	 
	 
	if (st->st_mtime < archive_entry_mtime(entry))
		return (1);
	 
	if (st->st_mtime > archive_entry_mtime(entry))
		return (0);
	 
#if HAVE_STRUCT_STAT_ST_MTIMESPEC_TV_NSEC
	 
	if (st->st_mtimespec.tv_nsec < archive_entry_mtime_nsec(entry))
		return (1);
#elif HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC
	 
	if (st->st_mtim.tv_nsec < archive_entry_mtime_nsec(entry))
		return (1);
#elif HAVE_STRUCT_STAT_ST_MTIME_N
	 
	if (st->st_mtime_n < archive_entry_mtime_nsec(entry))
		return (1);
#elif HAVE_STRUCT_STAT_ST_UMTIME
	 
	if (st->st_umtime * 1000 < archive_entry_mtime_nsec(entry))
		return (1);
#elif HAVE_STRUCT_STAT_ST_MTIME_USEC
	 
	if (st->st_mtime_usec * 1000 < archive_entry_mtime_nsec(entry))
		return (1);
#else
	 
#endif
	 
	return (0);
}
