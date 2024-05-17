static void print_tags(blkid_dev dev, char *show[], int output)
{
	blkid_tag_iterate	iter;
	const char		*type, *value, *devname;
	int			num = 1;
	static int		first = 1;

	if (!dev)
		return;

	if (output & OUTPUT_PRETTY_LIST) {
		pretty_print_dev(dev);
		return;
	}

	devname = blkid_dev_devname(dev);

	if (output & OUTPUT_DEVICE_ONLY) {
		printf("%s\n", devname);
		return;
	}

	iter = blkid_tag_iterate_begin(dev);
	while (blkid_tag_next(iter, &type, &value) == 0) {
		if (show[0] && !has_item(show, type))
			continue;

		if (num == 1 && !first &&
		    (output & (OUTPUT_UDEV_LIST | OUTPUT_EXPORT_LIST)))
			 
			fputc('\n', stdout);

		print_value(output, num++, devname, value, type, strlen(value));
	}
	blkid_tag_iterate_end(iter);

	if (num > 1) {
		if (!(output & (OUTPUT_VALUE_ONLY | OUTPUT_UDEV_LIST |
						OUTPUT_EXPORT_LIST)))
			printf("\n");
		first = 0;
	}
}