sd2_open (SF_PRIVATE *psf)
{	int subformat, error = 0, valid ;

	 
	psf->endian = SF_ENDIAN_BIG ;

	if (psf->file.mode == SFM_READ || (psf->file.mode == SFM_RDWR && psf->rsrclength > 0))
	{	psf_use_rsrc (psf, SF_TRUE) ;
		valid = psf_file_valid (psf) ;
		psf_use_rsrc (psf, SF_FALSE) ;
		if (! valid)
		{	psf_log_printf (psf, "sd2_open : psf->rsrc.filedes < 0\n") ;
			return SFE_SD2_BAD_RSRC ;
			} ;

		error = sd2_parse_rsrc_fork (psf) ;

		if (error)
			goto error_cleanup ;
		} ;

	if ((SF_CONTAINER (psf->sf.format)) != SF_FORMAT_SD2)
	{	error = SFE_BAD_OPEN_FORMAT ;
		goto error_cleanup ;
		} ;

	subformat = SF_CODEC (psf->sf.format) ;
	psf->dataoffset = 0 ;

	 
	if (psf->file.mode == SFM_WRITE || (psf->file.mode == SFM_RDWR && psf->rsrclength == 0))
	{	psf->rsrc.mode = psf->file.mode ;
		psf_open_rsrc (psf) ;

		error = sd2_write_rsrc_fork (psf, SF_FALSE) ;

		if (error)
			goto error_cleanup ;

		 
		psf->write_header = NULL ;
		} ;

	psf->container_close = sd2_close ;

	psf->blockwidth = psf->bytewidth * psf->sf.channels ;

	switch (subformat)
	{	case SF_FORMAT_PCM_S8 :	 
		case SF_FORMAT_PCM_16 :	 
		case SF_FORMAT_PCM_24 :	 
		case SF_FORMAT_PCM_32 :	 
				error = pcm_init (psf) ;
				break ;

		default :
				error = SFE_UNIMPLEMENTED ;
				break ;
		} ;

	psf_fseek (psf, psf->dataoffset, SEEK_SET) ;

error_cleanup:

	 
	psf_close_rsrc (psf) ;

	return error ;
}  
