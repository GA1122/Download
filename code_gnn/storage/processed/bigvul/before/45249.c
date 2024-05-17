psf_open_rsrc (SF_PRIVATE *psf)
{
	if (psf->rsrc.filedes > 0)
		return 0 ;

	 
	snprintf (psf->rsrc.path.c, sizeof (psf->rsrc.path.c), "%s/..namedfork/rsrc", psf->file.path.c) ;
	psf->error = SFE_NO_ERROR ;
	if ((psf->rsrc.filedes = psf_open_fd (&psf->rsrc)) >= 0)
	{	psf->rsrclength = psf_get_filelen_fd (psf->rsrc.filedes) ;
		if (psf->rsrclength > 0 || (psf->rsrc.mode & SFM_WRITE))
			return SFE_NO_ERROR ;
		psf_close_fd (psf->rsrc.filedes) ;
		psf->rsrc.filedes = -1 ;
		} ;

	if (psf->rsrc.filedes == - SFE_BAD_OPEN_MODE)
	{	psf->error = SFE_BAD_OPEN_MODE ;
		return psf->error ;
		} ;

	 
	snprintf (psf->rsrc.path.c, sizeof (psf->rsrc.path.c), "%s._%s", psf->file.dir.c, psf->file.name.c) ;
	psf->error = SFE_NO_ERROR ;
	if ((psf->rsrc.filedes = psf_open_fd (&psf->rsrc)) >= 0)
	{	psf->rsrclength = psf_get_filelen_fd (psf->rsrc.filedes) ;
		return SFE_NO_ERROR ;
		} ;

	 
	snprintf (psf->rsrc.path.c, sizeof (psf->rsrc.path.c), "%s.AppleDouble/%s", psf->file.dir.c, psf->file.name.c) ;
	psf->error = SFE_NO_ERROR ;
	if ((psf->rsrc.filedes = psf_open_fd (&psf->rsrc)) >= 0)
	{	psf->rsrclength = psf_get_filelen_fd (psf->rsrc.filedes) ;
		return SFE_NO_ERROR ;
		} ;

	 
	if (psf->rsrc.filedes == -1)
		psf_log_syserr (psf, errno) ;

	psf->rsrc.filedes = -1 ;

	return psf->error ;
}  
