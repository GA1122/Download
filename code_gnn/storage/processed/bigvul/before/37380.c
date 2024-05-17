static bool __check_direct_spte_mmio_pf(u64 spte)
{
	 
	return spte == 0ull;
}
