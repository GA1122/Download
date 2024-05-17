static void refill_stock(struct mem_cgroup *memcg, unsigned int nr_pages)
{
	struct memcg_stock_pcp *stock = &get_cpu_var(memcg_stock);

	if (stock->cached != memcg) {  
		drain_stock(stock);
		stock->cached = memcg;
	}
	stock->nr_pages += nr_pages;
	put_cpu_var(memcg_stock);
}