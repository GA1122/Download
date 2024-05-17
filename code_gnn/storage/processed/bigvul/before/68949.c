static void slab_put_obj(struct kmem_cache *cachep,
			struct page *page, void *objp)
{
	unsigned int objnr = obj_to_index(cachep, page, objp);
#if DEBUG
	unsigned int i;

	 
	for (i = page->active; i < cachep->num; i++) {
		if (get_free_obj(page, i) == objnr) {
			pr_err("slab: double free detected in cache '%s', objp %p\n",
			       cachep->name, objp);
			BUG();
		}
	}
#endif
	page->active--;
	if (!page->freelist)
		page->freelist = objp + obj_offset(cachep);

	set_free_obj(page, page->active, objnr);
}