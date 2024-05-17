 struct o2nm_node *o2nm_get_node_by_num(u8 node_num)
 {
 	struct o2nm_node *node = NULL;

	if (node_num >= O2NM_MAX_NODES || o2nm_single_cluster == NULL)
		goto out;

	read_lock(&o2nm_single_cluster->cl_nodes_lock);
	node = o2nm_single_cluster->cl_nodes[node_num];
	if (node)
		config_item_get(&node->nd_item);
	read_unlock(&o2nm_single_cluster->cl_nodes_lock);
out:
	return node;
}