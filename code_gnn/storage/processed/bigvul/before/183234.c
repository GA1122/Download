 header_seek (SF_PRIVATE *psf, sf_count_t position, int whence)
  {
  	switch (whence)
  	{	case SEEK_SET :
			if (position > SIGNED_SIZEOF (psf->header))
// 			if (psf->header.indx + position >= psf->header.len)
// 				psf_bump_header_allocation (psf, position) ;
// 			if (position > psf->header.len)
  			{	 
  				psf_fseek (psf, position, whence) ;
  				return ;
  				} ;
			if (position > psf->headend)
				psf->headend += psf_fread (psf->header + psf->headend, 1, position - psf->headend, psf) ;
			psf->headindex = position ;
// 			if (position > psf->header.end)
// 				psf->header.end += psf_fread (psf->header.ptr + psf->header.end, 1, position - psf->header.end, psf) ;
// 			psf->header.indx = position ;
  			break ;
  
  		case SEEK_CUR :
			if (psf->headindex + position < 0)
// 			if (psf->header.indx + position >= psf->header.len)
// 				psf_bump_header_allocation (psf, position) ;
// 
// 			if (psf->header.indx + position < 0)
  				break ;
  
			if (psf->headindex >= SIGNED_SIZEOF (psf->header))
// 			if (psf->header.indx >= psf->header.len)
  			{	psf_fseek (psf, position, whence) ;
  				return ;
  				} ;
  
			if (psf->headindex + position <= psf->headend)
			{	psf->headindex += position ;
// 			if (psf->header.indx + position <= psf->header.end)
// 			{	psf->header.indx += position ;
  				break ;
  				} ;
  
			if (psf->headindex + position > SIGNED_SIZEOF (psf->header))
// 			if (psf->header.indx + position > psf->header.len)
  			{	 
				psf->headindex = psf->headend ;
// 				psf->header.indx = psf->header.end ;
  				psf_fseek (psf, position, SEEK_CUR) ;
  				break ;
  				} ;
  
			psf->headend += psf_fread (psf->header + psf->headend, 1, position - (psf->headend - psf->headindex), psf) ;
			psf->headindex = psf->headend ;
// 			psf->header.end += psf_fread (psf->header.ptr + psf->header.end, 1, position - (psf->header.end - psf->header.indx), psf) ;
// 			psf->header.indx = psf->header.end ;
  			break ;
  
  		case SEEK_END :
 		default :
 			psf_log_printf (psf, "Bad whence param in header_seek().\n") ;
 			break ;
 		} ;
 
 	return ;
 }  