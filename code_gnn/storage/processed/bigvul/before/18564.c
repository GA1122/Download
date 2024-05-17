ext4_ext_more_to_rm(struct ext4_ext_path *path)
{
	BUG_ON(path->p_idx == NULL);

	if (path->p_idx < EXT_FIRST_INDEX(path->p_hdr))
		return 0;

	 
	if (le16_to_cpu(path->p_hdr->eh_entries) == path->p_block)
		return 0;
	return 1;
}