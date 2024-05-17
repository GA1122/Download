static int lowprobe_topology(blkid_probe pr)
{
	 
	blkid_probe_enable_topology(pr, 1);

	blkid_probe_enable_superblocks(pr, 0);
	blkid_probe_enable_partitions(pr, 0);

	return blkid_do_fullprobe(pr);
}
