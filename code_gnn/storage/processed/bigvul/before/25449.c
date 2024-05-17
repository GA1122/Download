static bool pmc_overflow(unsigned long val)
{
	if ((int)val < 0)
		return true;

	 
	if (__is_processor(PV_POWER7) && ((0x80000000 - val) <= 256))
		return true;

	return false;
}
