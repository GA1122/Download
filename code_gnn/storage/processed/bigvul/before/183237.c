 psf_binheader_writef (SF_PRIVATE *psf, const char *format, ...)
 {	va_list	argptr ;
 	sf_count_t 		countdata ;
 	unsigned long 	longdata ;
 	unsigned int 	data ;
 	float			floatdata ;
 	double			doubledata ;
 	void			*bindata ;
 	size_t			size ;
 	char			c, *strptr ;
 	int				count = 0, trunc_8to4 ;
 
 	trunc_8to4 = SF_FALSE ;
 
  	va_start (argptr, format) ;
  
  	while ((c = *format++))
	{	switch (c)
// 	{
// 		if (psf->header.indx + 16 >= psf->header.len && psf_bump_header_allocation (psf, 16))
// 			return count ;
// 
// 		switch (c)
  		{	case ' ' :  
  					break ;
  
 			case 'e' :  
 					psf->rwf_endian = SF_ENDIAN_LITTLE ;
 					break ;
 
 			case 'E' :  
 					psf->rwf_endian = SF_ENDIAN_BIG ;
 					break ;
 
 			case 't' :  
 					trunc_8to4 = SF_TRUE ;
 					break ;
 
 			case 'T' :  
 					trunc_8to4 = SF_FALSE ;
 					break ;
 
 			case 'm' :
 					data = va_arg (argptr, unsigned int) ;
 					header_put_marker (psf, data) ;
 					count += 4 ;
 					break ;
 
 			case '1' :
 					data = va_arg (argptr, unsigned int) ;
 					header_put_byte (psf, data) ;
 					count += 1 ;
 					break ;
 
 			case '2' :
 					data = va_arg (argptr, unsigned int) ;
 					if (psf->rwf_endian == SF_ENDIAN_BIG)
 					{	header_put_be_short (psf, data) ;
 						}
 					else
 					{	header_put_le_short (psf, data) ;
 						} ;
 					count += 2 ;
 					break ;
 
 			case '3' :  
 					data = va_arg (argptr, unsigned int) ;
 					if (psf->rwf_endian == SF_ENDIAN_BIG)
 					{	header_put_be_3byte (psf, data) ;
 						}
 					else
 					{	header_put_le_3byte (psf, data) ;
 						} ;
 					count += 3 ;
 					break ;
 
 			case '4' :
 					data = va_arg (argptr, unsigned int) ;
 					if (psf->rwf_endian == SF_ENDIAN_BIG)
 					{	header_put_be_int (psf, data) ;
 						}
 					else
 					{	header_put_le_int (psf, data) ;
 						} ;
 					count += 4 ;
 					break ;
 
 			case '8' :
 					countdata = va_arg (argptr, sf_count_t) ;
 					if (psf->rwf_endian == SF_ENDIAN_BIG && trunc_8to4 == SF_FALSE)
 					{	header_put_be_8byte (psf, countdata) ;
 						count += 8 ;
 						}
 					else if (psf->rwf_endian == SF_ENDIAN_LITTLE && trunc_8to4 == SF_FALSE)
 					{	header_put_le_8byte (psf, countdata) ;
 						count += 8 ;
 						}
 					else if (psf->rwf_endian == SF_ENDIAN_BIG && trunc_8to4 == SF_TRUE)
 					{	longdata = countdata & 0xFFFFFFFF ;
 						header_put_be_int (psf, longdata) ;
 						count += 4 ;
 						}
 					else if (psf->rwf_endian == SF_ENDIAN_LITTLE && trunc_8to4 == SF_TRUE)
 					{	longdata = countdata & 0xFFFFFFFF ;
 						header_put_le_int (psf, longdata) ;
 						count += 4 ;
 						}
 					break ;
 
 			case 'f' :
  					 
  					floatdata = (float) va_arg (argptr, double) ;
  					if (psf->rwf_endian == SF_ENDIAN_BIG)
						float32_be_write (floatdata, psf->header + psf->headindex) ;
// 						float32_be_write (floatdata, psf->header.ptr + psf->header.indx) ;
  					else
						float32_le_write (floatdata, psf->header + psf->headindex) ;
					psf->headindex += 4 ;
// 						float32_le_write (floatdata, psf->header.ptr + psf->header.indx) ;
// 					psf->header.indx += 4 ;
  					count += 4 ;
  					break ;
  
  			case 'd' :
  					doubledata = va_arg (argptr, double) ;
  					if (psf->rwf_endian == SF_ENDIAN_BIG)
						double64_be_write (doubledata, psf->header + psf->headindex) ;
// 						double64_be_write (doubledata, psf->header.ptr + psf->header.indx) ;
  					else
						double64_le_write (doubledata, psf->header + psf->headindex) ;
					psf->headindex += 8 ;
// 						double64_le_write (doubledata, psf->header.ptr + psf->header.indx) ;
// 					psf->header.indx += 8 ;
  					count += 8 ;
  					break ;
  
 			case 's' :
 					 
  					strptr = va_arg (argptr, char *) ;
  					size = strlen (strptr) + 1 ;
  					size += (size & 1) ;
// 
// 					if (psf->header.indx + (sf_count_t) size >= psf->header.len && psf_bump_header_allocation (psf, 16))
// 						return count ;
// 
  					if (psf->rwf_endian == SF_ENDIAN_BIG)
  						header_put_be_int (psf, size) ;
  					else
  						header_put_le_int (psf, size) ;
					memcpy (&(psf->header [psf->headindex]), strptr, size) ;
					psf->headindex += size ;
					psf->header [psf->headindex - 1] = 0 ;
// 					memcpy (&(psf->header.ptr [psf->header.indx]), strptr, size) ;
// 					psf->header.indx += size ;
// 					psf->header.ptr [psf->header.indx - 1] = 0 ;
  					count += 4 + size ;
  					break ;
  
 			case 'S' :
 					 
  					strptr = va_arg (argptr, char *) ;
  					size = strlen (strptr) ;
// 					if (psf->header.indx + (sf_count_t) size > psf->header.len && psf_bump_header_allocation (psf, size))
// 						return count ;
  					if (psf->rwf_endian == SF_ENDIAN_BIG)
  						header_put_be_int (psf, size) ;
  					else
  						header_put_le_int (psf, size) ;
					memcpy (&(psf->header [psf->headindex]), strptr, size + 1) ;
// 					memcpy (&(psf->header.ptr [psf->header.indx]), strptr, size + 1) ;
  					size += (size & 1) ;
					psf->headindex += size ;
					psf->header [psf->headindex] = 0 ;
// 					psf->header.indx += size ;
// 					psf->header.ptr [psf->header.indx] = 0 ;
  					count += 4 + size ;
  					break ;
  
 			case 'p' :
 					 
 					strptr = va_arg (argptr, char *) ;
  					size = strlen (strptr) ;
  					size = (size & 1) ? size : size + 1 ;
  					size = (size > 254) ? 254 : size ;
// 
// 					if (psf->header.indx + (sf_count_t) size > psf->header.len && psf_bump_header_allocation (psf, size))
// 						return count ;
// 
  					header_put_byte (psf, size) ;
					memcpy (&(psf->header [psf->headindex]), strptr, size) ;
					psf->headindex += size ;
// 					memcpy (&(psf->header.ptr [psf->header.indx]), strptr, size) ;
// 					psf->header.indx += size ;
  					count += 1 + size ;
  					break ;
  
  			case 'b' :
  					bindata	= va_arg (argptr, void *) ;
  					size	= va_arg (argptr, size_t) ;
					if (psf->headindex + size < sizeof (psf->header))
					{	memcpy (&(psf->header [psf->headindex]), bindata, size) ;
						psf->headindex += size ;
						count += size ;
						} ;
// 
// 					if (psf->header.indx + (sf_count_t) size > psf->header.len && psf_bump_header_allocation (psf, size))
// 						return count ;
// 
// 					memcpy (&(psf->header.ptr [psf->header.indx]), bindata, size) ;
// 					psf->header.indx += size ;
// 					count += size ;
  					break ;
  
  			case 'z' :
  					size = va_arg (argptr, size_t) ;
// 
// 					if (psf->header.indx + (sf_count_t) size > psf->header.len && psf_bump_header_allocation (psf, size))
// 						return count ;
// 
  					count += size ;
  					while (size)
					{	psf->header [psf->headindex] = 0 ;
						psf->headindex ++ ;
// 					{	psf->header.ptr [psf->header.indx] = 0 ;
// 						psf->header.indx ++ ;
  						size -- ;
  						} ;
  					break ;
  
  			case 'h' :
  					bindata = va_arg (argptr, void *) ;
					memcpy (&(psf->header [psf->headindex]), bindata, 16) ;
					psf->headindex += 16 ;
// 					memcpy (&(psf->header.ptr [psf->header.indx]), bindata, 16) ;
// 					psf->header.indx += 16 ;
  					count += 16 ;
  					break ;
  
  			case 'j' :	 
  					size = va_arg (argptr, size_t) ;
					psf->headindex += size ;
// 
// 					if (psf->header.indx + (sf_count_t) size > psf->header.len && psf_bump_header_allocation (psf, size))
// 						return count ;
// 
// 					psf->header.indx += size ;
  					count += size ;
  					break ;
  
  			case 'o' :	 
  					size = va_arg (argptr, size_t) ;
					if (size < sizeof (psf->header))
					{	psf->headindex = size ;
						count = 0 ;
						} ;
// 
// 					if ((sf_count_t) size >= psf->header.len && psf_bump_header_allocation (psf, size))
// 						return count ;
// 
// 					psf->header.indx = size ;
  					break ;
  
  			default :
 				psf_log_printf (psf, "*** Invalid format specifier `%c'\n", c) ;
 				psf->error = SFE_INTERNAL ;
 				break ;
 			} ;
 		} ;
 
 	va_end (argptr) ;
 	return count ;
 }  