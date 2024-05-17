flac_seek (SF_PRIVATE *psf, int UNUSED (mode), sf_count_t offset)
{	FLAC_PRIVATE* pflac = (FLAC_PRIVATE*) psf->codec_data ;

	if (pflac == NULL)
		return 0 ;

	if (psf->dataoffset < 0)
	{	psf->error = SFE_BAD_SEEK ;
		return ((sf_count_t) -1) ;
		} ;

	pflac->frame = NULL ;

	if (psf->file.mode == SFM_READ)
	{	if (FLAC__stream_decoder_seek_absolute (pflac->fsd, offset))
			return offset ;

		if (offset == psf->sf.frames)
		{	 
			return offset ;
			} ;

		psf->error = SFE_BAD_SEEK ;
		return ((sf_count_t) -1) ;
		} ;

	 
	psf->error = SFE_BAD_SEEK ;

	return ((sf_count_t) -1) ;
}  
