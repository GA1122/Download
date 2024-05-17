static int page_trans_compound_anon_split(struct page *page)
{
	int ret = 0;
	struct page *transhuge_head = page_trans_compound_anon(page);
	if (transhuge_head) {
		 
		if (get_page_unless_zero(transhuge_head)) {
			 
			if (PageAnon(transhuge_head))
				ret = split_huge_page(transhuge_head);
			else
				 
				ret = 1;
			put_page(transhuge_head);
		} else
			 
			ret = 1;
	}
	return ret;
}
