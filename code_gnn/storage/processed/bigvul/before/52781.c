static int find_overflow_devnum(void)
{
	int ret;

	if (!overflow_maj) {
		ret = alloc_chrdev_region(&overflow_maj, 0, IB_UCM_MAX_DEVICES,
					  "infiniband_cm");
		if (ret) {
			pr_err("ucm: couldn't register dynamic device number\n");
			return ret;
		}
	}

	ret = find_first_zero_bit(overflow_map, IB_UCM_MAX_DEVICES);
	if (ret >= IB_UCM_MAX_DEVICES)
		return -1;

	return ret;
}
