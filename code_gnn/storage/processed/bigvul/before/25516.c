void fault_init(void)
{
	if (test_facility(2) && test_facility(75))
		store_indication = 0xc00;
}
