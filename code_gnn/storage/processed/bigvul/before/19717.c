static void hugetlb_unregister_all_nodes(void)
{
	int nid;

	 
	register_hugetlbfs_with_node(NULL, NULL);

	 
	for (nid = 0; nid < nr_node_ids; nid++)
		hugetlb_unregister_node(&node_devices[nid]);
}
