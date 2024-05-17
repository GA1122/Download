size_t ksize(const void *objp)
{
	size_t size;

	BUG_ON(!objp);
	if (unlikely(objp == ZERO_SIZE_PTR))
		return 0;

	size = virt_to_cache(objp)->object_size;
	 
	kasan_unpoison_shadow(objp, size);

	return size;
}
