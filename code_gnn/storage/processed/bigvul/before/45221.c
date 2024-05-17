psf_fseek (SF_PRIVATE *psf, sf_count_t offset, int whence)
{	sf_count_t	current_pos, new_position ;

	if (psf->virtual_io)
		return psf->vio.seek (offset, whence, psf->vio_user_data) ;

	current_pos = psf_ftell (psf) ;

	switch (whence)
	{	case SEEK_SET :
				offset += psf->fileoffset ;
				break ;

		case SEEK_END :
				if (psf->file.mode == SFM_WRITE)
				{	new_position = lseek (psf->file.filedes, offset, whence) ;

					if (new_position < 0)
						psf_log_syserr (psf, errno) ;

					return new_position - psf->fileoffset ;
					} ;

				 
				whence = SEEK_SET ;
				offset = lseek (psf->file.filedes, 0, SEEK_END) + offset ;
				break ;

		case SEEK_CUR :
				 
				offset += current_pos ;
				whence = SEEK_SET ;
				break ;

		default :
				 
				psf_log_printf (psf, "psf_fseek : whence is %d *****.\n", whence) ;
				return 0 ;
		} ;

	if (current_pos != offset)
		new_position = lseek (psf->file.filedes, offset, whence) ;
	else
		new_position = offset ;

	if (new_position < 0)
		psf_log_syserr (psf, errno) ;

	new_position -= psf->fileoffset ;

	return new_position ;
}  
