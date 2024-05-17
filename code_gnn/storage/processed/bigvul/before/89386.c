int part_get_info_whole_disk(struct blk_desc *dev_desc, disk_partition_t *info)
{
	info->start = 0;
	info->size = dev_desc->lba;
	info->blksz = dev_desc->blksz;
	info->bootable = 0;
	strcpy((char *)info->type, BOOT_PART_TYPE);
	strcpy((char *)info->name, "Whole Disk");
#if CONFIG_IS_ENABLED(PARTITION_UUIDS)
	info->uuid[0] = 0;
#endif
#ifdef CONFIG_PARTITION_TYPE_GUID
	info->type_guid[0] = 0;
#endif

	return 0;
}
