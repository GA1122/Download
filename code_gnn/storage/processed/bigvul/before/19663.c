int __weak alloc_bootmem_huge_page(struct hstate *h)
{
	struct huge_bootmem_page *m;
	int nr_nodes = nodes_weight(node_states[N_HIGH_MEMORY]);

	while (nr_nodes) {
		void *addr;

		addr = __alloc_bootmem_node_nopanic(
				NODE_DATA(hstate_next_node_to_alloc(h,
						&node_states[N_HIGH_MEMORY])),
				huge_page_size(h), huge_page_size(h), 0);

		if (addr) {
			 
			m = addr;
			goto found;
		}
		nr_nodes--;
	}
	return 0;

found:
	BUG_ON((unsigned long)virt_to_phys(m) & (huge_page_size(h) - 1));
	 
	list_add(&m->list, &huge_boot_pages);
	m->hstate = h;
	return 1;
}