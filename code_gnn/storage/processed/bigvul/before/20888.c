static bool valid_pat_type(unsigned t)
{
	return t < 8 && (1 << t) & 0xf3;  
}
