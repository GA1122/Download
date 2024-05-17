_tiffSeekProc(thandle_t fd, uint64 off, int whence)
{
	off_t off_io = (off_t) off;
	if ((uint64) off_io != off)
	{
		errno=EINVAL;
		return (uint64) -1;  
	}
	return((uint64)lseek((int)fd,off_io,whence));
}
