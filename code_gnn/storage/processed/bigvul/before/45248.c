psf_open_handle (PSF_FILE * pfile)
{	DWORD dwDesiredAccess ;
	DWORD dwShareMode ;
	DWORD dwCreationDistribution ;
	HANDLE handle ;

	switch (pfile->mode)
	{	case SFM_READ :
				dwDesiredAccess = GENERIC_READ ;
				dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE ;
				dwCreationDistribution = OPEN_EXISTING ;
				break ;

		case SFM_WRITE :
				dwDesiredAccess = GENERIC_WRITE ;
				dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE ;
				dwCreationDistribution = CREATE_ALWAYS ;
				break ;

		case SFM_RDWR :
				dwDesiredAccess = GENERIC_READ | GENERIC_WRITE ;
				dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE ;
				dwCreationDistribution = OPEN_ALWAYS ;
				break ;

		default :
				return NULL ;
		} ;

	if (pfile->use_wchar)
		handle = CreateFileW (
					pfile->path.wc,				 
					dwDesiredAccess,			 
					dwShareMode,				 
					0,							 
					dwCreationDistribution,		 
					FILE_ATTRIBUTE_NORMAL,		 
					NULL						 
					) ;
	else
		handle = CreateFile (
					pfile->path.c,				 
					dwDesiredAccess,			 
					dwShareMode,				 
					0,							 
					dwCreationDistribution,		 
					FILE_ATTRIBUTE_NORMAL,		 
					NULL						 
					) ;

	if (handle == INVALID_HANDLE_VALUE)
		return NULL ;

	return handle ;
}  
