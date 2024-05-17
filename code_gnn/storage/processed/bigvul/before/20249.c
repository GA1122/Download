 static void prep_new_huge_page(struct hstate *h, struct page *page, int nid)
{
	set_compound_page_dtor(page, free_huge_page);
	spin_lock(&hugetlb_lock);
	h->nr_huge_pages++;
	h->nr_huge_pages_node[nid]++;
	spin_unlock(&hugetlb_lock);
	put_page(page);  
}