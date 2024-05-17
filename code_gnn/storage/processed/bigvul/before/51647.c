clear_nochange_fflags(struct archive_write_disk *a)
{
	int		nochange_flags;
	mode_t		mode = archive_entry_mode(a->entry);

	 
	nochange_flags = 0;
#ifdef SF_IMMUTABLE
	nochange_flags |= SF_IMMUTABLE;
#endif
#ifdef UF_IMMUTABLE
	nochange_flags |= UF_IMMUTABLE;
#endif
#ifdef SF_APPEND
	nochange_flags |= SF_APPEND;
#endif
#ifdef UF_APPEND
	nochange_flags |= UF_APPEND;
#endif
#ifdef EXT2_APPEND_FL
	nochange_flags |= EXT2_APPEND_FL;
#endif
#ifdef EXT2_IMMUTABLE_FL
	nochange_flags |= EXT2_IMMUTABLE_FL;
#endif

	return (set_fflags_platform(a, a->fd, a->name, mode, 0, nochange_flags));
}