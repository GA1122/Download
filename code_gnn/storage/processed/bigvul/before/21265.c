void tlb_gather_mmu(struct mmu_gather *tlb, struct mm_struct *mm, bool fullmm)
{
	tlb->mm = mm;

	tlb->fullmm     = fullmm;
	tlb->need_flush = 0;
	tlb->fast_mode  = (num_possible_cpus() == 1);
	tlb->local.next = NULL;
	tlb->local.nr   = 0;
	tlb->local.max  = ARRAY_SIZE(tlb->__pages);
	tlb->active     = &tlb->local;

#ifdef CONFIG_HAVE_RCU_TABLE_FREE
	tlb->batch = NULL;
#endif
}