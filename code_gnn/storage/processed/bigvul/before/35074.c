static void __split_huge_page(struct page *page,
			      struct anon_vma *anon_vma)
{
	int mapcount, mapcount2;
	struct anon_vma_chain *avc;

	BUG_ON(!PageHead(page));
	BUG_ON(PageTail(page));

	mapcount = 0;
	list_for_each_entry(avc, &anon_vma->head, same_anon_vma) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long addr = vma_address(page, vma);
		BUG_ON(is_vma_temporary_stack(vma));
		if (addr == -EFAULT)
			continue;
		mapcount += __split_huge_page_splitting(page, vma, addr);
	}
	 
	if (mapcount != page_mapcount(page))
		printk(KERN_ERR "mapcount %d page_mapcount %d\n",
		       mapcount, page_mapcount(page));
	BUG_ON(mapcount != page_mapcount(page));

	__split_huge_page_refcount(page);

	mapcount2 = 0;
	list_for_each_entry(avc, &anon_vma->head, same_anon_vma) {
		struct vm_area_struct *vma = avc->vma;
		unsigned long addr = vma_address(page, vma);
		BUG_ON(is_vma_temporary_stack(vma));
		if (addr == -EFAULT)
			continue;
		mapcount2 += __split_huge_page_map(page, vma, addr);
	}
	if (mapcount != mapcount2)
		printk(KERN_ERR "mapcount %d mapcount2 %d page_mapcount %d\n",
		       mapcount, mapcount2, page_mapcount(page));
	BUG_ON(mapcount != mapcount2);
}