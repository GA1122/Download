static inline int ext2_feature_set_ok(struct super_block *sb)
{
	if (ext4_has_unknown_ext2_incompat_features(sb))
		return 0;
	if (sb->s_flags & MS_RDONLY)
		return 1;
	if (ext4_has_unknown_ext2_ro_compat_features(sb))
		return 0;
	return 1;
}
