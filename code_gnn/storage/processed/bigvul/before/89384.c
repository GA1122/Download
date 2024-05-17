int part_get_info_by_name(struct blk_desc *dev_desc, const char *name,
			  disk_partition_t *info)
{
	return part_get_info_by_name_type(dev_desc, name, info, PART_TYPE_ALL);
}
