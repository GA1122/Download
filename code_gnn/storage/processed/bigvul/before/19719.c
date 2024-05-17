void hugetlb_unregister_node(struct node *node)
{
	struct hstate *h;
	struct node_hstate *nhs = &node_hstates[node->dev.id];

	if (!nhs->hugepages_kobj)
		return;		 

	for_each_hstate(h)
		if (nhs->hstate_kobjs[h - hstates]) {
			kobject_put(nhs->hstate_kobjs[h - hstates]);
			nhs->hstate_kobjs[h - hstates] = NULL;
		}

	kobject_put(nhs->hugepages_kobj);
	nhs->hugepages_kobj = NULL;
}