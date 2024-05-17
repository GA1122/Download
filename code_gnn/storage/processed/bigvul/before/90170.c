static int ext4_find_delayed_extent(struct inode *inode,
				    struct extent_status *newes)
{
	struct extent_status es;
	ext4_lblk_t block, next_del;

	if (newes->es_pblk == 0) {
		ext4_es_find_extent_range(inode, &ext4_es_is_delayed,
					  newes->es_lblk,
					  newes->es_lblk + newes->es_len - 1,
					  &es);

		 
		if (es.es_len == 0)
			 
			return 0;

		if (es.es_lblk > newes->es_lblk) {
			 
			newes->es_len = min(es.es_lblk - newes->es_lblk,
					    newes->es_len);
			return 0;
		}

		newes->es_len = es.es_lblk + es.es_len - newes->es_lblk;
	}

	block = newes->es_lblk + newes->es_len;
	ext4_es_find_extent_range(inode, &ext4_es_is_delayed, block,
				  EXT_MAX_BLOCKS, &es);
	if (es.es_len == 0)
		next_del = EXT_MAX_BLOCKS;
	else
		next_del = es.es_lblk;

	return next_del;
}