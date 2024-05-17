static unsigned long super_cache_scan(struct shrinker *shrink,
				      struct shrink_control *sc)
{
	struct super_block *sb;
	long	fs_objects = 0;
	long	total_objects;
	long	freed = 0;
	long	dentries;
	long	inodes;

	sb = container_of(shrink, struct super_block, s_shrink);

	 
	if (!(sc->gfp_mask & __GFP_FS))
		return SHRINK_STOP;

	if (!grab_super_passive(sb))
		return SHRINK_STOP;

	if (sb->s_op->nr_cached_objects)
		fs_objects = sb->s_op->nr_cached_objects(sb, sc->nid);

	inodes = list_lru_count_node(&sb->s_inode_lru, sc->nid);
	dentries = list_lru_count_node(&sb->s_dentry_lru, sc->nid);
	total_objects = dentries + inodes + fs_objects + 1;

	 
	dentries = mult_frac(sc->nr_to_scan, dentries, total_objects);
	inodes = mult_frac(sc->nr_to_scan, inodes, total_objects);

	 
	freed = prune_dcache_sb(sb, dentries, sc->nid);
	freed += prune_icache_sb(sb, inodes, sc->nid);

	if (fs_objects) {
		fs_objects = mult_frac(sc->nr_to_scan, fs_objects,
								total_objects);
		freed += sb->s_op->free_cached_objects(sb, fs_objects,
						       sc->nid);
	}

	drop_super(sb);
	return freed;
}