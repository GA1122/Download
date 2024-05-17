static struct part_driver *part_driver_lookup_type(struct blk_desc *dev_desc)
{
	struct part_driver *drv =
		ll_entry_start(struct part_driver, part_driver);
	const int n_ents = ll_entry_count(struct part_driver, part_driver);
	struct part_driver *entry;

	if (dev_desc->part_type == PART_TYPE_UNKNOWN) {
		for (entry = drv; entry != drv + n_ents; entry++) {
			int ret;

			ret = entry->test(dev_desc);
			if (!ret) {
				dev_desc->part_type = entry->part_type;
				return entry;
			}
		}
	} else {
		for (entry = drv; entry != drv + n_ents; entry++) {
			if (dev_desc->part_type == entry->part_type)
				return entry;
		}
	}

	 
	return NULL;
}
