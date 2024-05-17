static void finalize_super_block(struct exfat* ef)
{
	if (ef->ro)
		return;

	ef->sb->volume_state = cpu_to_le16(
			le16_to_cpu(ef->sb->volume_state) & ~EXFAT_STATE_MOUNTED);

	 
	if (ef->sb->allocated_percent != 0xff)
	{
		uint32_t free, total;

		free = exfat_count_free_clusters(ef);
		total = le32_to_cpu(ef->sb->cluster_count);
		ef->sb->allocated_percent = ((total - free) * 100 + total / 2) / total;
	}

	commit_super_block(ef);	 
}
