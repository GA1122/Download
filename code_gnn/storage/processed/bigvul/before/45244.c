psf_log_syserr (SF_PRIVATE *psf, int error)
{
	 
	if (psf->error == 0)
	{	psf->error = SFE_SYSTEM ;
		snprintf (psf->syserr, sizeof (psf->syserr), "System error : %s.", strerror (error)) ;
		} ;

	return ;
}  
