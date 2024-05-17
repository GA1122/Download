 static int gup_huge_pud(pud_t orig, pud_t *pudp, unsigned long addr,
 		unsigned long end, int write, struct page **pages, int *nr)
 {
 	struct page *head, *page;
 	int refs;
 
 	if (!pud_access_permitted(orig, write))
 		return 0;
 
 	if (pud_devmap(orig))
 		return __gup_device_huge_pud(orig, pudp, addr, end, pages, nr);
 
 	refs = 0;
 	page = pud_page(orig) + ((addr & ~PUD_MASK) >> PAGE_SHIFT);
 	do {
 		pages[*nr] = page;
 		(*nr)++;
 		page++;
  		refs++;
  	} while (addr += PAGE_SIZE, addr != end);
  
	head = compound_head(pud_page(orig));
	if (!page_cache_add_speculative(head, refs)) {
// 	head = try_get_compound_head(pud_page(orig), refs);
// 	if (!head) {
  		*nr -= refs;
  		return 0;
  	}
 
 	if (unlikely(pud_val(orig) != pud_val(*pudp))) {
 		*nr -= refs;
 		while (refs--)
 			put_page(head);
 		return 0;
 	}
 
 	SetPageReferenced(head);
 	return 1;
 }