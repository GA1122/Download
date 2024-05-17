void local_flush_tlb_one(unsigned long asid, unsigned long page)
{
	unsigned long long match, pteh=0, lpage;
	unsigned long tlb;

	 
	lpage = neff_sign_extend(page);
	match = (asid << PTEH_ASID_SHIFT) | PTEH_VALID;
	match |= lpage;

	for_each_itlb_entry(tlb) {
		asm volatile ("getcfg	%1, 0, %0"
			      : "=r" (pteh)
			      : "r" (tlb) );

		if (pteh == match) {
			__flush_tlb_slot(tlb);
			break;
		}
	}

	for_each_dtlb_entry(tlb) {
		asm volatile ("getcfg	%1, 0, %0"
			      : "=r" (pteh)
			      : "r" (tlb) );

		if (pteh == match) {
			__flush_tlb_slot(tlb);
			break;
		}

	}
}