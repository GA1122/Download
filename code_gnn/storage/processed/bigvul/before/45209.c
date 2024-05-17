psf_fclose (SF_PRIVATE *psf)
{	int retval ;

	while ((retval = close (psf->file.filedes)) == -1 && errno == EINTR)
		  ;

	if (retval == -1)
		psf_log_syserr (psf, errno) ;

	psf->file.filedes = -1 ;

	return retval ;
}  
