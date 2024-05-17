static inline unsigned short twd_i387_to_fxsr(unsigned short twd)
{
	unsigned int tmp;  

	 
	tmp = ~twd;
	tmp = (tmp | (tmp>>1)) & 0x5555;  
	 
	tmp = (tmp | (tmp >> 1)) & 0x3333;  
	tmp = (tmp | (tmp >> 2)) & 0x0f0f;  
	tmp = (tmp | (tmp >> 4)) & 0x00ff;  

	return tmp;
}
