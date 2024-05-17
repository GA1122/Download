static ssize_t nr_hugepages_store_common(bool obey_mempolicy,
			struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t len)
{
	int err;
	int nid;
	unsigned long count;
	struct hstate *h;
	NODEMASK_ALLOC(nodemask_t, nodes_allowed, GFP_KERNEL | __GFP_NORETRY);

	err = strict_strtoul(buf, 10, &count);
	if (err)
		goto out;

	h = kobj_to_hstate(kobj, &nid);
	if (h->order >= MAX_ORDER) {
		err = -EINVAL;
		goto out;
	}

	if (nid == NUMA_NO_NODE) {
		 
		if (!(obey_mempolicy &&
				init_nodemask_of_mempolicy(nodes_allowed))) {
			NODEMASK_FREE(nodes_allowed);
			nodes_allowed = &node_states[N_HIGH_MEMORY];
		}
	} else if (nodes_allowed) {
		 
		count += h->nr_huge_pages - h->nr_huge_pages_node[nid];
		init_nodemask_of_node(nodes_allowed, nid);
	} else
		nodes_allowed = &node_states[N_HIGH_MEMORY];

	h->max_huge_pages = set_max_huge_pages(h, count, nodes_allowed);

	if (nodes_allowed != &node_states[N_HIGH_MEMORY])
		NODEMASK_FREE(nodes_allowed);

	return len;
out:
	NODEMASK_FREE(nodes_allowed);
	return err;
}