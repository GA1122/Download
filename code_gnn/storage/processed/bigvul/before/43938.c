set_times(struct archive_write_disk *a,
    int fd, int mode, const char *name,
    time_t atime, long atime_nanos,
    time_t birthtime, long birthtime_nanos,
    time_t mtime, long mtime_nanos,
    time_t cctime, long ctime_nanos)
{
	 
	int r1 = 0, r2 = 0;

#ifdef F_SETTIMES
	  
	if (a->user_uid == 0 &&
	    set_time_tru64(fd, mode, name,
			   atime, atime_nanos, mtime,
			   mtime_nanos, cctime, ctime_nanos) == 0) {
		return (ARCHIVE_OK);
	}
#else  
	(void)cctime;  
	(void)ctime_nanos;  
#endif  

#ifdef HAVE_STRUCT_STAT_ST_BIRTHTIME
	 
	if (birthtime < mtime
	    || (birthtime == mtime && birthtime_nanos < mtime_nanos))
		r1 = set_time(fd, mode, name,
			      atime, atime_nanos,
			      birthtime, birthtime_nanos);
#else
	(void)birthtime;  
	(void)birthtime_nanos;  
#endif
	r2 = set_time(fd, mode, name,
		      atime, atime_nanos,
		      mtime, mtime_nanos);
	if (r1 != 0 || r2 != 0) {
		archive_set_error(&a->archive, errno,
				  "Can't restore time");
		return (ARCHIVE_WARN);
	}
	return (ARCHIVE_OK);
}
