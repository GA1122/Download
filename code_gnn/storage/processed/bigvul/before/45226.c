psf_ftell (SF_PRIVATE *psf)
{	sf_count_t pos ;

	if (psf->virtual_io)
		return psf->vio.tell (psf->vio_user_data) ;

	if (psf->is_pipe)
		return psf->pipeoffset ;

	pos = lseek (psf->file.filedes, 0, SEEK_CUR) ;

	if (pos == ((sf_count_t) -1))
	{	psf_log_syserr (psf, errno) ;
		return -1 ;
		} ;

	return pos - psf->fileoffset ;
}  
