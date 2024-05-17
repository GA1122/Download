tenbytefloat2int (uint8_t *bytes)
{	int val = 3 ;

	if (bytes [0] & 0x80)	 
		return 0 ;

	if (bytes [0] <= 0x3F)	 
		return 1 ;

	if (bytes [0] > 0x40)	 
		return 0x4000000 ;

	if (bytes [0] == 0x40 && bytes [1] > 0x1C)  
		return 800000000 ;

	 

	val = (bytes [2] << 23) | (bytes [3] << 15) | (bytes [4] << 7) | (bytes [5] >> 1) ;

	val >>= (29 - bytes [1]) ;

	return val ;
}  
