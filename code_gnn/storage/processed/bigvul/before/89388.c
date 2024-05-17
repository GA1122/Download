void part_print(struct blk_desc *dev_desc)
{
	struct part_driver *drv;

	drv = part_driver_lookup_type(dev_desc);
	if (!drv) {
		printf("## Unknown partition table type %x\n",
		       dev_desc->part_type);
		return;
	}

	PRINTF("## Testing for valid %s partition ##\n", drv->name);
	print_part_header(drv->name, dev_desc);
	if (drv->print)
		drv->print(dev_desc);
}
