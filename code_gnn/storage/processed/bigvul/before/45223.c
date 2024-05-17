psf_fseek (SF_PRIVATE *psf, sf_count_t offset, int whence)
{	sf_count_t	new_position ;

	if (psf->virtual_io)
		return psf->vio.seek (offset, whence, psf->vio_user_data) ;

	switch (whence)
	{	case SEEK_SET :
				offset += psf->fileoffset ;
				break ;

		case SEEK_END :
				if (psf->file.mode == SFM_WRITE)
				{	new_position = _lseeki64 (psf->file.filedes, offset, whence) ;

					if (new_position < 0)
						psf_log_syserr (psf, errno) ;

					return new_position - psf->fileoffset ;
					} ;

				 
				whence = SEEK_SET ;
				offset = _lseeki64 (psf->file.filedes, 0, SEEK_END) + offset ;
				break ;

		default :
				 
				break ;
		} ;

	 
	if (offset == 0 && whence == SEEK_CUR)
		new_position = _telli64 (psf->file.filedes) ;
	else
		new_position = _lseeki64 (psf->file.filedes, offset, whence) ;

	if (new_position < 0)
		psf_log_syserr (psf, errno) ;

	new_position -= psf->fileoffset ;

	return new_position ;
}  
