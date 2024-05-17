static void o2nm_node_group_drop_item(struct config_group *group,
				      struct config_item *item)
{
	struct o2nm_node *node = to_o2nm_node(item);
	struct o2nm_cluster *cluster = to_o2nm_cluster(group->cg_item.ci_parent);

	o2net_disconnect_node(node);

	if (cluster->cl_has_local &&
	    (cluster->cl_local_node == node->nd_num)) {
		cluster->cl_has_local = 0;
		cluster->cl_local_node = O2NM_INVALID_NODE_NUM;
		o2net_stop_listening(node);
	}

	 

	write_lock(&cluster->cl_nodes_lock);

	 
	if (node->nd_ipv4_address)
		rb_erase(&node->nd_ip_node, &cluster->cl_node_ip_tree);

	 
	if (cluster->cl_nodes[node->nd_num] == node) {
		cluster->cl_nodes[node->nd_num] = NULL;
		clear_bit(node->nd_num, cluster->cl_nodes_bitmap);
	}
	write_unlock(&cluster->cl_nodes_lock);

	mlog(ML_CLUSTER, "o2nm: Unregistered node %s\n",
	     config_item_name(&node->nd_item));

	config_item_put(item);
}