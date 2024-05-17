struct lruvec *mem_cgroup_lru_move_lists(struct zone *zone,
					 struct page *page,
					 enum lru_list from,
					 enum lru_list to)
{
	 
	mem_cgroup_lru_del_list(page, from);
	return mem_cgroup_lru_add_list(zone, page, to);
}