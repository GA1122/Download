void local_flush_tlb_all(void)
{
	 
	unsigned long flags, tlb;

	local_irq_save(flags);

	 
	for_each_itlb_entry(tlb)
		__flush_tlb_slot(tlb);

	 
	for_each_dtlb_entry(tlb)
		__flush_tlb_slot(tlb);

	local_irq_restore(flags);
}
