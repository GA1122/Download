struct page *alloc_pages_current(gfp_t gfp, unsigned order)
{
	struct mempolicy *pol = &default_policy;
	struct page *page;
	unsigned int cpuset_mems_cookie;

	if (!in_interrupt() && !(gfp & __GFP_THISNODE))
		pol = get_task_policy(current);

retry_cpuset:
	cpuset_mems_cookie = read_mems_allowed_begin();

	 
	if (pol->mode == MPOL_INTERLEAVE)
		page = alloc_page_interleave(gfp, order, interleave_nodes(pol));
	else
		page = __alloc_pages_nodemask(gfp, order,
				policy_zonelist(gfp, pol, numa_node_id()),
				policy_nodemask(gfp, pol));

	if (unlikely(!page && read_mems_allowed_retry(cpuset_mems_cookie)))
		goto retry_cpuset;

	return page;
}