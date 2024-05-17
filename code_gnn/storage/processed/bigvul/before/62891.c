static bool is_valid_cache(u32 val)
{
	u32 level, ctype;

	if (val >= CSSELR_MAX)
		return false;

	 
	level = (val >> 1);
	ctype = (cache_levels >> (level * 3)) & 7;

	switch (ctype) {
	case 0:  
		return false;
	case 1:  
		return (val & 1);
	case 2:  
	case 4:  
		return !(val & 1);
	case 3:  
		return true;
	default:  
		return false;
	}
}
