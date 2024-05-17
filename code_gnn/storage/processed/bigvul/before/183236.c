 psf_binheader_readf (SF_PRIVATE *psf, char const *format, ...)
 {	va_list			argptr ;
 	sf_count_t		*countptr, countdata ;
 	unsigned char	*ucptr, sixteen_bytes [16] ;
 	unsigned int 	*intptr, intdata ;
 	unsigned short	*shortptr ;
 	char			*charptr ;
  	float			*floatptr ;
  	double			*doubleptr ;
  	char			c ;
	int				byte_count = 0, count ;
// 	int				byte_count = 0, count = 0 ;
  
  	if (! format)
  		return psf_ftell (psf) ;
  
  	va_start (argptr, format) ;
  
  	while ((c = *format++))
	{	switch (c)
// 	{
// 		if (psf->header.indx + 16 >= psf->header.len && psf_bump_header_allocation (psf, 16))
// 			return count ;
// 
// 		switch (c)
  		{	case 'e' :  
  					psf->rwf_endian = SF_ENDIAN_LITTLE ;
  					break ;
 
 			case 'E' :  
 					psf->rwf_endian = SF_ENDIAN_BIG ;
 					break ;
  
  			case 'm' :  
  					intptr = va_arg (argptr, unsigned int*) ;
// 					*intptr = 0 ;
  					ucptr = (unsigned char*) intptr ;
  					byte_count += header_read (psf, ucptr, sizeof (int)) ;
  					*intptr = GET_MARKER (ucptr) ;
  					break ;
  
  			case 'h' :
  					intptr = va_arg (argptr, unsigned int*) ;
// 					*intptr = 0 ;
  					ucptr = (unsigned char*) intptr ;
  					byte_count += header_read (psf, sixteen_bytes, sizeof (sixteen_bytes)) ;
  					{	int k ;
 						intdata = 0 ;
 						for (k = 0 ; k < 16 ; k++)
 							intdata ^= sixteen_bytes [k] << k ;
 						}
 					*intptr = intdata ;
 					break ;
 
 			case '1' :
 					charptr = va_arg (argptr, char*) ;
 					*charptr = 0 ;
 					byte_count += header_read (psf, charptr, sizeof (char)) ;
 					break ;
 
 			case '2' :  
 					shortptr = va_arg (argptr, unsigned short*) ;
 					*shortptr = 0 ;
 					ucptr = (unsigned char*) shortptr ;
 					byte_count += header_read (psf, ucptr, sizeof (short)) ;
 					if (psf->rwf_endian == SF_ENDIAN_BIG)
 						*shortptr = GET_BE_SHORT (ucptr) ;
 					else
 						*shortptr = GET_LE_SHORT (ucptr) ;
 					break ;
 
 			case '3' :  
 					intptr = va_arg (argptr, unsigned int*) ;
 					*intptr = 0 ;
 					byte_count += header_read (psf, sixteen_bytes, 3) ;
 					if (psf->rwf_endian == SF_ENDIAN_BIG)
 						*intptr = GET_BE_3BYTE (sixteen_bytes) ;
 					else
 						*intptr = GET_LE_3BYTE (sixteen_bytes) ;
 					break ;
 
 			case '4' :  
 					intptr = va_arg (argptr, unsigned int*) ;
 					*intptr = 0 ;
 					ucptr = (unsigned char*) intptr ;
 					byte_count += header_read (psf, ucptr, sizeof (int)) ;
 					if (psf->rwf_endian == SF_ENDIAN_BIG)
 						*intptr = psf_get_be32 (ucptr, 0) ;
 					else
 						*intptr = psf_get_le32 (ucptr, 0) ;
 					break ;
 
 			case '8' :  
 					countptr = va_arg (argptr, sf_count_t *) ;
 					*countptr = 0 ;
 					byte_count += header_read (psf, sixteen_bytes, 8) ;
 					if (psf->rwf_endian == SF_ENDIAN_BIG)
 						countdata = psf_get_be64 (sixteen_bytes, 0) ;
 					else
 						countdata = psf_get_le64 (sixteen_bytes, 0) ;
 					*countptr = countdata ;
 					break ;
 
 			case 'f' :  
 					floatptr = va_arg (argptr, float *) ;
 					*floatptr = 0.0 ;
 					byte_count += header_read (psf, floatptr, sizeof (float)) ;
 					if (psf->rwf_endian == SF_ENDIAN_BIG)
 						*floatptr = float32_be_read ((unsigned char*) floatptr) ;
 					else
 						*floatptr = float32_le_read ((unsigned char*) floatptr) ;
 					break ;
 
 			case 'd' :  
 					doubleptr = va_arg (argptr, double *) ;
 					*doubleptr = 0.0 ;
 					byte_count += header_read (psf, doubleptr, sizeof (double)) ;
 					if (psf->rwf_endian == SF_ENDIAN_BIG)
 						*doubleptr = double64_be_read ((unsigned char*) doubleptr) ;
 					else
 						*doubleptr = double64_le_read ((unsigned char*) doubleptr) ;
 					break ;
 
 			case 's' :
 					psf_log_printf (psf, "Format conversion 's' not implemented yet.\n") ;
 					 
  					break ;
  
  			case 'b' :  
  					charptr = va_arg (argptr, char*) ;
  					count = va_arg (argptr, size_t) ;
					if (count > 0)
						byte_count += header_read (psf, charptr, count) ;
// 					memset (charptr, 0, count) ;
// 					byte_count += header_read (psf, charptr, count) ;
  					break ;
  
  			case 'G' :
  					charptr = va_arg (argptr, char*) ;
  					count = va_arg (argptr, size_t) ;
					if (count > 0)
						byte_count += header_gets (psf, charptr, count) ;
// 					memset (charptr, 0, count) ;
// 
// 					if (psf->header.indx + count >= psf->header.len && psf_bump_header_allocation (psf, count))
// 						return 0 ;
// 
// 					byte_count += header_gets (psf, charptr, count) ;
  					break ;
  
  			case 'z' :
  					psf_log_printf (psf, "Format conversion 'z' not implemented yet.\n") ;
  					 
  					break ;
  
			case 'p' :
					 
// 			case 'p' :	 
  					count = va_arg (argptr, size_t) ;
  					header_seek (psf, count, SEEK_SET) ;
  					byte_count = count ;
  					break ;
  
			case 'j' :
					 
// 			case 'j' :	 
  					count = va_arg (argptr, size_t) ;
					if (count)
					{	header_seek (psf, count, SEEK_CUR) ;
						byte_count += count ;
						} ;
// 					header_seek (psf, count, SEEK_CUR) ;
// 					byte_count += count ;
  					break ;
  
  			default :
 				psf_log_printf (psf, "*** Invalid format specifier `%c'\n", c) ;
 				psf->error = SFE_INTERNAL ;
 				break ;
 			} ;
 		} ;
 
 	va_end (argptr) ;
 
 	return byte_count ;
 }  