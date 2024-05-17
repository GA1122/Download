static struct page *page_trans_compound_anon(struct page *page)
{
	if (PageTransCompound(page)) {
		struct page *head = compound_trans_head(page);
		 
		if (PageAnon(head))
			return head;
	}
	return NULL;
}
