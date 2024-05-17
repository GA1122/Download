static int ubifs_set_page_dirty(struct page *page)
{
	int ret;

	ret = __set_page_dirty_nobuffers(page);
	 
	ubifs_assert(ret == 0);
	return ret;
}
