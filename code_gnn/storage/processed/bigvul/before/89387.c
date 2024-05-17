void part_init(struct blk_desc *dev_desc)
{
	struct part_driver *drv =
		ll_entry_start(struct part_driver, part_driver);
	const int n_ents = ll_entry_count(struct part_driver, part_driver);
	struct part_driver *entry;

	blkcache_invalidate(dev_desc->if_type, dev_desc->devnum);

	dev_desc->part_type = PART_TYPE_UNKNOWN;
	for (entry = drv; entry != drv + n_ents; entry++) {
		int ret;

		ret = entry->test(dev_desc);
		debug("%s: try '%s': ret=%d\n", __func__, entry->name, ret);
		if (!ret) {
			dev_desc->part_type = entry->part_type;
			break;
		}
	}
}
