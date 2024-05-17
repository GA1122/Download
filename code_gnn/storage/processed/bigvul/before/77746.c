void __dump_page(struct page *page, const char *reason)
{
	bool page_poisoned = PagePoisoned(page);
	int mapcount;

	 
	if (page_poisoned) {
		pr_emerg("page:%px is uninitialized and poisoned", page);
		goto hex_only;
	}

	 
	mapcount = PageSlab(page) ? 0 : page_mapcount(page);

	pr_emerg("page:%px count:%d mapcount:%d mapping:%px index:%#lx",
		  page, page_ref_count(page), mapcount,
		  page->mapping, page_to_pgoff(page));
	if (PageCompound(page))
		pr_cont(" compound_mapcount: %d", compound_mapcount(page));
	pr_cont("\n");
	BUILD_BUG_ON(ARRAY_SIZE(pageflag_names) != __NR_PAGEFLAGS + 1);

	pr_emerg("flags: %#lx(%pGp)\n", page->flags, &page->flags);

hex_only:
	print_hex_dump(KERN_ALERT, "raw: ", DUMP_PREFIX_NONE, 32,
			sizeof(unsigned long), page,
			sizeof(struct page), false);

	if (reason)
		pr_alert("page dumped because: %s\n", reason);

#ifdef CONFIG_MEMCG
	if (!page_poisoned && page->mem_cgroup)
		pr_alert("page->mem_cgroup:%px\n", page->mem_cgroup);
#endif
}
