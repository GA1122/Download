static void undo_dev_pagemap(int *nr, int nr_start, struct page **pages)
{
	while ((*nr) - nr_start) {
		struct page *page = pages[--(*nr)];

		ClearPageReferenced(page);
		put_page(page);
 	}
 }