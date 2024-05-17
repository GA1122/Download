static bool seq_nr_after(u16 a, u16 b)
{
	 
	if ((int) b - a == 32768)
		return false;

	return (((s16) (b - a)) < 0);
}
