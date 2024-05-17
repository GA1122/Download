psf_strlcpy_crlf (char *dest, const char *src, size_t destmax, size_t srcmax)
{	 
	char * destend = dest + destmax - 2 ;
	const char * srcend = src + srcmax ;

	while (dest < destend && src < srcend)
	{	if ((src [0] == '\r' && src [1] == '\n') || (src [0] == '\n' && src [1] == '\r'))
		{	*dest++ = '\r' ;
			*dest++ = '\n' ;
			src += 2 ;
			continue ;
			} ;

		if (src [0] == '\r')
		{	*dest++ = '\r' ;
			*dest++ = '\n' ;
			src += 1 ;
			continue ;
			} ;

		if (src [0] == '\n')
		{	*dest++ = '\r' ;
			*dest++ = '\n' ;
			src += 1 ;
			continue ;
			} ;

		*dest++ = *src++ ;
		} ;

	 
	*dest = 0 ;
}  
