static int alloc_area(struct pstore *ps)
{
	int r = -ENOMEM;
	size_t len;

	len = ps->store->chunk_size << SECTOR_SHIFT;

	 
	ps->area = vmalloc(len);
	if (!ps->area)
		goto err_area;

	ps->zero_area = vzalloc(len);
	if (!ps->zero_area)
		goto err_zero_area;

	ps->header_area = vmalloc(len);
	if (!ps->header_area)
		goto err_header_area;

	return 0;

err_header_area:
	vfree(ps->zero_area);

err_zero_area:
	vfree(ps->area);

err_area:
	return r;
}