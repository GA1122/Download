static void handle_slab(unsigned long *n, struct kmem_cache *c,
						struct page *page)
{
	void *p;
	int i, j;
	unsigned long v;

	if (n[0] == n[1])
		return;
	for (i = 0, p = page->s_mem; i < c->num; i++, p += c->size) {
		bool active = true;

		for (j = page->active; j < c->num; j++) {
			if (get_free_obj(page, j) == i) {
				active = false;
				break;
			}
		}

		if (!active)
			continue;

		 
		if (probe_kernel_read(&v, dbg_userword(c, p), sizeof(v)))
			continue;

		if (!add_caller(n, v))
			return;
	}
}
