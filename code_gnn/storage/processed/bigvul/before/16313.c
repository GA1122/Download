static int statfs_string( char *line, struct statvfs *info )
#else
static int statfs_string( char *line, struct statfs *info )
#endif
{
#ifdef WIN32
	return 0;
#else
	return sprintf(line,"%lld %lld %lld %lld %lld %lld %lld\n",
#  if defined(Solaris)
		(long long) info->f_fsid,
		(long long) info->f_frsize,
#  else
		(long long) info->f_type,
		(long long) info->f_bsize,
#  endif
		(long long) info->f_blocks,
		(long long) info->f_bfree,
		(long long) info->f_bavail,
		(long long) info->f_files,
		(long long) info->f_ffree
	);
#endif
}
