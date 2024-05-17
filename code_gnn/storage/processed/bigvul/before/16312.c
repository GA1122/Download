static int stat_string( char *line, struct stat *info )
{
#ifdef WIN32
	return 0;
#else
	return sprintf(line,"%lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld\n",
		(long long) info->st_dev,
		(long long) info->st_ino,
		(long long) info->st_mode,
		(long long) info->st_nlink,
		(long long) info->st_uid,
		(long long) info->st_gid,
		(long long) info->st_rdev,
		(long long) info->st_size,
		(long long) info->st_blksize,
		(long long) info->st_blocks,
		(long long) info->st_atime,
		(long long) info->st_mtime,
		(long long) info->st_ctime
	);
#endif
}
