static void count_spte_clear(u64 *sptep, u64 spte)
{
	struct kvm_mmu_page *sp =  page_header(__pa(sptep));

	if (is_shadow_present_pte(spte))
		return;

	 
	smp_wmb();
	sp->clear_spte_count++;
}
