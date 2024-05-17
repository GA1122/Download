int part_get_info_by_name_type(struct blk_desc *dev_desc, const char *name,
			       disk_partition_t *info, int part_type)
{
	struct part_driver *part_drv;
	int ret;
	int i;

	part_drv = part_driver_lookup_type(dev_desc);
	if (!part_drv)
		return -1;
	for (i = 1; i < part_drv->max_entries; i++) {
		ret = part_drv->get_info(dev_desc, i, info);
		if (ret != 0) {
			 
			break;
		}
		if (strcmp(name, (const char *)info->name) == 0) {
			 
			return i;
		}
	}

	return -1;
}
