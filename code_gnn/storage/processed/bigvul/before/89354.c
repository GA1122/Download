static int do_zfs_ls(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *filename = "/";
	int part;
	struct blk_desc *dev_desc;
	disk_partition_t info;
	struct device_s vdev;

	if (argc < 2)
		return cmd_usage(cmdtp);

	if (argc == 4)
		filename = argv[3];

	part = blk_get_device_part_str(argv[1], argv[2], &dev_desc, &info, 1);
	if (part < 0)
		return 1;

	zfs_set_blk_dev(dev_desc, &info);
	vdev.part_length = info.size;

	zfs_ls(&vdev, filename,
		   zfs_print);

	return 0;
}
