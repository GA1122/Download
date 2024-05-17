static void __init hugetlb_init_hstates(void)
{
	struct hstate *h;

	for_each_hstate(h) {
		 
		if (h->order < MAX_ORDER)
			hugetlb_hstate_alloc_pages(h);
	}
}
