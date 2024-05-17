int isolate_movable_page(struct page *page, isolate_mode_t mode)
{
	struct address_space *mapping;

	 
	if (unlikely(!get_page_unless_zero(page)))
		goto out;

	 
	if (unlikely(!__PageMovable(page)))
		goto out_putpage;
	 
	if (unlikely(!trylock_page(page)))
		goto out_putpage;

	if (!PageMovable(page) || PageIsolated(page))
		goto out_no_isolated;

	mapping = page_mapping(page);
	VM_BUG_ON_PAGE(!mapping, page);

	if (!mapping->a_ops->isolate_page(page, mode))
		goto out_no_isolated;

	 
	WARN_ON_ONCE(PageIsolated(page));
	__SetPageIsolated(page);
	unlock_page(page);

	return 0;

out_no_isolated:
	unlock_page(page);
out_putpage:
	put_page(page);
out:
	return -EBUSY;
}
