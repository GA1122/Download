psf_close_fd (int fd)
{	int retval ;

	if (fd < 0)
		return 0 ;

	while ((retval = close (fd)) == -1 && errno == EINTR)
		  ;

	return retval ;
}  
