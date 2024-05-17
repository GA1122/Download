 static int gup_huge_pgd(pgd_t orig, pgd_t *pgdp, unsigned long addr,
 			unsigned long end, int write,
 			struct page **pages, int *nr)
 {
 	int refs;
 	struct page *head, *page;
 
 	if (!pgd_access_permitted(orig, write))
 		return 0;
 
 	BUILD_BUG_ON(pgd_devmap(orig));
 	refs = 0;
 	page = pgd_page(orig) + ((addr & ~PGDIR_MASK) >> PAGE_SHIFT);
 	do {
 		pages[*nr] = page;
 		(*nr)++;
 		page++;
  		refs++;
  	} while (addr += PAGE_SIZE, addr != end);
  
	head = compound_head(pgd_page(orig));
	if (!page_cache_add_speculative(head, refs)) {
// 	head = try_get_compound_head(pgd_page(orig), refs);
// 	if (!head) {
  		*nr -= refs;
  		return 0;
  	}
 
 	if (unlikely(pgd_val(orig) != pgd_val(*pgdp))) {
 		*nr -= refs;
 		while (refs--)
 			put_page(head);
 		return 0;
 	}
 
 	SetPageReferenced(head);
 	return 1;
 }