 int o2nm_depend_item(struct config_item *item)
 {
 	return configfs_depend_item(&o2nm_cluster_group.cs_subsys, item);
}