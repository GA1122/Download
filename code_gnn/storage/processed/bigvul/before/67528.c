int ext4_get_next_extent(struct inode *inode, ext4_lblk_t lblk,
			 unsigned int map_len, struct extent_status *result)
{
	struct ext4_map_blocks map;
	struct extent_status es = {};
	int ret;

	map.m_lblk = lblk;
	map.m_len = map_len;

	 
	while (map.m_len > 0) {
		ret = ext4_map_blocks(NULL, inode, &map, 0);
		if (ret < 0)
			return ret;
		 
		if (ret > 0) {
			int status;

			ext4_es_store_pblock(result, map.m_pblk);
			result->es_lblk = map.m_lblk;
			result->es_len = map.m_len;
			if (map.m_flags & EXT4_MAP_UNWRITTEN)
				status = EXTENT_STATUS_UNWRITTEN;
			else
				status = EXTENT_STATUS_WRITTEN;
			ext4_es_store_status(result, status);
			return 1;
		}
		ext4_es_find_delayed_extent_range(inode, map.m_lblk,
						  map.m_lblk + map.m_len - 1,
						  &es);
		 
		if (es.es_len && es.es_lblk < map.m_lblk + map.m_len) {
			ext4_lblk_t offset = 0;

			if (es.es_lblk < lblk)
				offset = lblk - es.es_lblk;
			result->es_lblk = es.es_lblk + offset;
			ext4_es_store_pblock(result,
					     ext4_es_pblock(&es) + offset);
			result->es_len = es.es_len - offset;
			ext4_es_store_status(result, ext4_es_status(&es));

			return 1;
		}
		 
		map.m_lblk += map.m_len;
		map_len -= map.m_len;
		map.m_len = map_len;
		cond_resched();
	}
	result->es_len = 0;
	return 0;
}
