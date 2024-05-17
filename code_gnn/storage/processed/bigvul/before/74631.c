static int lowprobe_device(blkid_probe pr, const char *devname,
			int chain, char *show[], int output,
			blkid_loff_t offset, blkid_loff_t size)
{
	const char *data;
	const char *name;
	int nvals = 0, n, num = 1;
	size_t len;
	int fd;
	int rc = 0;
	static int first = 1;

	fd = open(devname, O_RDONLY|O_CLOEXEC);
	if (fd < 0) {
		fprintf(stderr, "error: %s: %m\n", devname);
		return BLKID_EXIT_NOTFOUND;
	}
	if (blkid_probe_set_device(pr, fd, offset, size))
		goto done;

	if (chain & LOWPROBE_TOPOLOGY)
		rc = lowprobe_topology(pr);
	if (rc >= 0 && (chain & LOWPROBE_SUPERBLOCKS))
		rc = lowprobe_superblocks(pr);
	if (rc < 0)
		goto done;

	if (!rc)
		nvals = blkid_probe_numof_values(pr);

	if (nvals &&
	    !(chain & LOWPROBE_TOPOLOGY) &&
	    !(output & OUTPUT_UDEV_LIST) &&
	    !blkid_probe_has_value(pr, "TYPE") &&
	    !blkid_probe_has_value(pr, "PTTYPE"))
		 
		nvals = 0;

	if (nvals && !first && output & (OUTPUT_UDEV_LIST | OUTPUT_EXPORT_LIST))
		 
		fputc('\n', stdout);

	if (nvals && (output & OUTPUT_DEVICE_ONLY)) {
		printf("%s\n", devname);
		goto done;
	}

	for (n = 0; n < nvals; n++) {
		if (blkid_probe_get_value(pr, n, &name, &data, &len))
			continue;
		if (show[0] && !has_item(show, name))
			continue;
		len = strnlen((char *) data, len);
		print_value(output, num++, devname, (char *) data, name, len);
	}

	if (first)
		first = 0;
	if (nvals >= 1 && !(output & (OUTPUT_VALUE_ONLY |
					OUTPUT_UDEV_LIST | OUTPUT_EXPORT_LIST)))
		printf("\n");
done:
	if (rc == -2) {
		if (output & OUTPUT_UDEV_LIST)
			print_udev_ambivalent(pr);
		else
			fprintf(stderr,
				"%s: ambivalent result (probably more "
				"filesystems on the device, use wipefs(8) "
				"to see more details)\n",
				devname);
	}
	close(fd);

	if (rc == -2)
		return BLKID_EXIT_AMBIVAL;	 
	if (!nvals)
		return BLKID_EXIT_NOTFOUND;	 

	return 0;		 
}