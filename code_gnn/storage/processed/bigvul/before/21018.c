static bool consume_stock(struct mem_cgroup *memcg)
{
	struct memcg_stock_pcp *stock;
	bool ret = true;

	stock = &get_cpu_var(memcg_stock);
	if (memcg == stock->cached && stock->nr_pages)
		stock->nr_pages--;
	else  
		ret = false;
	put_cpu_var(memcg_stock);
	return ret;
}
