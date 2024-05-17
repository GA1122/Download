void invalidate_bdev(struct block_device *bdev)
{
	struct address_space *mapping = bdev->bd_inode->i_mapping;

	if (mapping->nrpages == 0)
		return;

	invalidate_bh_lrus();
	lru_add_drain_all();	 
	invalidate_mapping_pages(mapping, 0, -1);
	 
	cleancache_invalidate_inode(mapping);
}
