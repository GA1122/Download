int kmem_ptr_validate(struct kmem_cache *s, const void *object)
{
	struct page *page;

	page = get_object_page(object);

	if (!page || s != page->slab)
		 
		return 0;

	if (!check_valid_pointer(s, page, object))
		return 0;

	 
	return 1;
}
