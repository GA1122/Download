psf_is_pipe (SF_PRIVATE *psf)
{	struct stat statbuf ;

	if (psf->virtual_io)
		return SF_FALSE ;

	 
	if (fstat (psf->file.filedes, &statbuf) == -1)
	{	psf_log_syserr (psf, errno) ;
		 
		return SF_TRUE ;
		} ;

	 
	if (S_ISFIFO (statbuf.st_mode) || S_ISSOCK (statbuf.st_mode))
		return SF_TRUE ;

	return SF_FALSE ;
}  
