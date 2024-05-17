static int adjust_pool_surplus(struct hstate *h, nodemask_t *nodes_allowed,
				int delta)
{
	int start_nid, next_nid;
	int ret = 0;

	VM_BUG_ON(delta != -1 && delta != 1);

	if (delta < 0)
		start_nid = hstate_next_node_to_alloc(h, nodes_allowed);
	else
		start_nid = hstate_next_node_to_free(h, nodes_allowed);
	next_nid = start_nid;

	do {
		int nid = next_nid;
		if (delta < 0)  {
			 
			if (!h->surplus_huge_pages_node[nid]) {
				next_nid = hstate_next_node_to_alloc(h,
								nodes_allowed);
				continue;
			}
		}
		if (delta > 0) {
			 
			if (h->surplus_huge_pages_node[nid] >=
						h->nr_huge_pages_node[nid]) {
				next_nid = hstate_next_node_to_free(h,
								nodes_allowed);
				continue;
			}
		}

		h->surplus_huge_pages += delta;
		h->surplus_huge_pages_node[nid] += delta;
		ret = 1;
		break;
	} while (next_nid != start_nid);

	return ret;
}