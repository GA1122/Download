static int free_pool_huge_page(struct hstate *h, nodemask_t *nodes_allowed,
							 bool acct_surplus)
{
	int start_nid;
	int next_nid;
	int ret = 0;

	start_nid = hstate_next_node_to_free(h, nodes_allowed);
	next_nid = start_nid;

	do {
		 
		if ((!acct_surplus || h->surplus_huge_pages_node[next_nid]) &&
		    !list_empty(&h->hugepage_freelists[next_nid])) {
			struct page *page =
				list_entry(h->hugepage_freelists[next_nid].next,
					  struct page, lru);
			list_del(&page->lru);
			h->free_huge_pages--;
			h->free_huge_pages_node[next_nid]--;
			if (acct_surplus) {
				h->surplus_huge_pages--;
				h->surplus_huge_pages_node[next_nid]--;
			}
			update_and_free_page(h, page);
			ret = 1;
			break;
		}
		next_nid = hstate_next_node_to_free(h, nodes_allowed);
	} while (next_nid != start_nid);

	return ret;
}