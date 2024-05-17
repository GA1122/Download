static void drain_all_stock_async(struct mem_cgroup *root_memcg)
{
	 
	if (!mutex_trylock(&percpu_charge_mutex))
		return;
	drain_all_stock(root_memcg, false);
	mutex_unlock(&percpu_charge_mutex);
}
