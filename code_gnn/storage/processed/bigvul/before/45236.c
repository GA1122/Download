psf_get_filelen (SF_PRIVATE *psf)
{
#if 0
	 
	struct _stati64 statbuf ;

	if (_fstati64 (psf->file.filedes, &statbuf))
	{	psf_log_syserr (psf, errno) ;
		return (sf_count_t) -1 ;
		} ;

	return statbuf.st_size ;
#else
	sf_count_t current, filelen ;

	if (psf->virtual_io)
		return psf->vio.get_filelen (psf->vio_user_data) ;

	if ((current = _telli64 (psf->file.filedes)) < 0)
	{	psf_log_syserr (psf, errno) ;
		return (sf_count_t) -1 ;
		} ;

	 

	_lseeki64 (psf->file.filedes, 0, SEEK_END) ;

	if ((filelen = _lseeki64 (psf->file.filedes, 0, SEEK_END)) < 0)
	{	psf_log_syserr (psf, errno) ;
		return (sf_count_t) -1 ;
		} ;

	if (filelen > current)
		_lseeki64 (psf->file.filedes, current, SEEK_SET) ;

	switch (psf->file.mode)
	{	case SFM_WRITE :
			filelen = filelen - psf->fileoffset ;
			break ;

		case SFM_READ :
			if (psf->fileoffset > 0 && psf->filelength > 0)
				filelen = psf->filelength ;
			break ;

		case SFM_RDWR :
			 
			break ;

		default :
			filelen = 0 ;
		} ;

	return filelen ;
#endif
}  
