FcDirCacheOpenFile (const FcChar8 *cache_file, struct stat *file_stat)
{
    int	fd;

#ifdef _WIN32
    if (FcStat (cache_file, file_stat) < 0)
        return -1;
#endif
    fd = FcOpen((char *) cache_file, O_RDONLY | O_BINARY);
    if (fd < 0)
	return fd;
#ifndef _WIN32
    if (fstat (fd, file_stat) < 0)
    {
	close (fd);
	return -1;
    }
#endif
    return fd;
}
