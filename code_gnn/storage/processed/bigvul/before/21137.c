int mem_cgroup_try_charge_swapin(struct mm_struct *mm,
				 struct page *page,
				 gfp_t mask, struct mem_cgroup **memcgp)
{
	struct mem_cgroup *memcg;
	int ret;

	*memcgp = NULL;

	if (mem_cgroup_disabled())
		return 0;

	if (!do_swap_account)
		goto charge_cur_mm;
	 
	if (!PageSwapCache(page))
		goto charge_cur_mm;
	memcg = try_get_mem_cgroup_from_page(page);
	if (!memcg)
		goto charge_cur_mm;
	*memcgp = memcg;
	ret = __mem_cgroup_try_charge(NULL, mask, 1, memcgp, true);
	css_put(&memcg->css);
	if (ret == -EINTR)
		ret = 0;
	return ret;
charge_cur_mm:
	if (unlikely(!mm))
		mm = &init_mm;
	ret = __mem_cgroup_try_charge(mm, mask, 1, memcgp, true);
	if (ret == -EINTR)
		ret = 0;
	return ret;
}
