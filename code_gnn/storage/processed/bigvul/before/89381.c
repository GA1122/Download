int part_get_info(struct blk_desc *dev_desc, int part,
		       disk_partition_t *info)
{
#ifdef CONFIG_HAVE_BLOCK_DEVICE
	struct part_driver *drv;

#if CONFIG_IS_ENABLED(PARTITION_UUIDS)
	 
	info->uuid[0] = 0;
#endif
#ifdef CONFIG_PARTITION_TYPE_GUID
	info->type_guid[0] = 0;
#endif

	drv = part_driver_lookup_type(dev_desc);
	if (!drv) {
		debug("## Unknown partition table type %x\n",
		      dev_desc->part_type);
		return -EPROTONOSUPPORT;
	}
	if (!drv->get_info) {
		PRINTF("## Driver %s does not have the get_info() method\n",
		       drv->name);
		return -ENOSYS;
	}
	if (drv->get_info(dev_desc, part, info) == 0) {
		PRINTF("## Valid %s partition found ##\n", drv->name);
		return 0;
	}
#endif  

	return -1;
}
