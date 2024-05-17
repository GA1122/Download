static int lowprobe_superblocks(blkid_probe pr)
{
	struct stat st;
	int rc, fd = blkid_probe_get_fd(pr);

	if (fd < 0 || fstat(fd, &st))
		return -1;

	blkid_probe_enable_partitions(pr, 1);

	if (!S_ISCHR(st.st_mode) && blkid_probe_get_size(pr) <= 1024 * 1440 &&
	    blkid_probe_is_wholedisk(pr)) {
		 
		blkid_probe_enable_superblocks(pr, 0);

		rc = blkid_do_fullprobe(pr);
		if (rc < 0)
			return rc;	 

		if (blkid_probe_lookup_value(pr, "PTTYPE", NULL, NULL) == 0)
			return 0;	 
	}

	blkid_probe_set_partitions_flags(pr, BLKID_PARTS_ENTRY_DETAILS);
	blkid_probe_enable_superblocks(pr, 1);

	return blkid_do_safeprobe(pr);
}