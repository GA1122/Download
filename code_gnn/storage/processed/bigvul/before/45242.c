psf_is_pipe (SF_PRIVATE *psf)
{
	if (psf->virtual_io)
		return SF_FALSE ;

	if (GetFileType (psf->file.handle) == FILE_TYPE_DISK)
		return SF_FALSE ;

	 
	return SF_TRUE ;
}  
