psf_open_rsrc (SF_PRIVATE *psf)
{
	if (psf->rsrc.handle != NULL)
		return 0 ;

	 
	snprintf (psf->rsrc.path.c, sizeof (psf->rsrc.path.c), "%s/rsrc", psf->file.path.c) ;
	psf->error = SFE_NO_ERROR ;
	if ((psf->rsrc.handle = psf_open_handle (&psf->rsrc)) != NULL)
	{	psf->rsrclength = psf_get_filelen_handle (psf->rsrc.handle) ;
		return SFE_NO_ERROR ;
		} ;

	 
	snprintf (psf->rsrc.path.c, sizeof (psf->rsrc.path.c), "%s._%s", psf->file.dir.c, psf->file.name.c) ;
	psf->error = SFE_NO_ERROR ;
	if ((psf->rsrc.handle = psf_open_handle (&psf->rsrc)) != NULL)
	{	psf->rsrclength = psf_get_filelen_handle (psf->rsrc.handle) ;
		return SFE_NO_ERROR ;
		} ;

	 
	snprintf (psf->rsrc.path.c, sizeof (psf->rsrc.path.c), "%s.AppleDouble/%s", psf->file.dir.c, psf->file.name.c) ;
	psf->error = SFE_NO_ERROR ;
	if ((psf->rsrc.handle = psf_open_handle (&psf->rsrc)) != NULL)
	{	psf->rsrclength = psf_get_filelen_handle (psf->rsrc.handle) ;
		return SFE_NO_ERROR ;
		} ;

	 
	if (psf->rsrc.handle == NULL)
		psf_log_syserr (psf, GetLastError ()) ;

	psf->rsrc.handle = NULL ;

	return psf->error ;
}  
