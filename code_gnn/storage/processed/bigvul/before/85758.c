static struct config_item *o2nm_node_group_make_item(struct config_group *group,
						     const char *name)
{
	struct o2nm_node *node = NULL;

	if (strlen(name) > O2NM_MAX_NAME_LEN)
		return ERR_PTR(-ENAMETOOLONG);

	node = kzalloc(sizeof(struct o2nm_node), GFP_KERNEL);
	if (node == NULL)
		return ERR_PTR(-ENOMEM);

	strcpy(node->nd_name, name);  
	config_item_init_type_name(&node->nd_item, name, &o2nm_node_type);
	spin_lock_init(&node->nd_lock);

	mlog(ML_CLUSTER, "o2nm: Registering node %s\n", name);

	return &node->nd_item;
}