static bool signed_sub_overflows(s64 a, s64 b)
{
	 
	s64 res = (s64)((u64)a - (u64)b);

	if (b < 0)
		return res < a;
 	return res > a;
 }