str_of_endianness (int end)
{	switch (end)
	{	CASE_NAME (SF_ENDIAN_BIG) ;
		CASE_NAME (SF_ENDIAN_LITTLE) ;
		CASE_NAME (SF_ENDIAN_CPU) ;
		default :
			break ;
		} ;

	 
	return "" ;
}  
