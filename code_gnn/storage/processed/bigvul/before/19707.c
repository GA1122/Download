static void hugetlb_register_all_nodes(void)
{
	int nid;

	for_each_node_state(nid, N_HIGH_MEMORY) {
		struct node *node = &node_devices[nid];
		if (node->dev.id == nid)
			hugetlb_register_node(node);
	}

	 
	register_hugetlbfs_with_node(hugetlb_register_node,
				     hugetlb_unregister_node);
}
