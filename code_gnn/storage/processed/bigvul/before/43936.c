set_time(int fd, int mode, const char *name,
    time_t atime, long atime_nsec,
    time_t mtime, long mtime_nsec)
{
	 
#if defined(HAVE_UTIMENSAT) && defined(HAVE_FUTIMENS)
	 
	struct timespec ts[2];
	(void)mode;  
	ts[0].tv_sec = atime;
	ts[0].tv_nsec = atime_nsec;
	ts[1].tv_sec = mtime;
	ts[1].tv_nsec = mtime_nsec;
	if (fd >= 0)
		return futimens(fd, ts);
	return utimensat(AT_FDCWD, name, ts, AT_SYMLINK_NOFOLLOW);

#elif HAVE_UTIMES
	 
	struct timeval times[2];

	times[0].tv_sec = atime;
	times[0].tv_usec = atime_nsec / 1000;
	times[1].tv_sec = mtime;
	times[1].tv_usec = mtime_nsec / 1000;

#ifdef HAVE_FUTIMES
	if (fd >= 0)
		return (futimes(fd, times));
#else
	(void)fd;  
#endif
#ifdef HAVE_LUTIMES
	(void)mode;  
	return (lutimes(name, times));
#else
	if (S_ISLNK(mode))
		return (0);
	return (utimes(name, times));
#endif

#elif defined(HAVE_UTIME)
	 
	struct utimbuf times;
	(void)fd;  
	(void)name;  
	(void)atime_nsec;  
	(void)mtime_nsec;  
	times.actime = atime;
	times.modtime = mtime;
	if (S_ISLNK(mode))
		return (ARCHIVE_OK);
	return (utime(name, &times));

#else
	 
	(void)fd;  
	(void)mode;  
	(void)name;  
	(void)atime_nsec;  
	(void)mtime_nsec;  
	return (ARCHIVE_WARN);
#endif
}
