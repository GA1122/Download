void __init inode_init_early(void)
{
	unsigned int loop;

	 
	if (hashdist)
		return;

	inode_hashtable =
		alloc_large_system_hash("Inode-cache",
					sizeof(struct hlist_head),
					ihash_entries,
					14,
					HASH_EARLY,
					&i_hash_shift,
					&i_hash_mask,
					0,
					0);

	for (loop = 0; loop < (1U << i_hash_shift); loop++)
		INIT_HLIST_HEAD(&inode_hashtable[loop]);
}