static void __init gather_bootmem_prealloc(void)
{
	struct huge_bootmem_page *m;

	list_for_each_entry(m, &huge_boot_pages, list) {
		struct hstate *h = m->hstate;
		struct page *page;

#ifdef CONFIG_HIGHMEM
		page = pfn_to_page(m->phys >> PAGE_SHIFT);
		free_bootmem_late((unsigned long)m,
				  sizeof(struct huge_bootmem_page));
#else
		page = virt_to_page(m);
#endif
		__ClearPageReserved(page);
		WARN_ON(page_count(page) != 1);
		prep_compound_huge_page(page, h->order);
		prep_new_huge_page(h, page, page_to_nid(page));
		 
		if (h->order > (MAX_ORDER - 1))
			totalram_pages += 1 << h->order;
	}
}
