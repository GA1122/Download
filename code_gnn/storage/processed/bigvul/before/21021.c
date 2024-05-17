static void drain_all_stock_sync(struct mem_cgroup *root_memcg)
{
	 
	mutex_lock(&percpu_charge_mutex);
	drain_all_stock(root_memcg, true);
	mutex_unlock(&percpu_charge_mutex);
}