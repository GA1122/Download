static int ext4_feature_set_ok(struct super_block *sb, int readonly)
{
	if (EXT4_HAS_INCOMPAT_FEATURE(sb, ~EXT4_FEATURE_INCOMPAT_SUPP)) {
		ext4_msg(sb, KERN_ERR,
			"Couldn't mount because of "
			"unsupported optional features (%x)",
			(le32_to_cpu(EXT4_SB(sb)->s_es->s_feature_incompat) &
			~EXT4_FEATURE_INCOMPAT_SUPP));
		return 0;
	}

	if (readonly)
		return 1;

	 
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb, ~EXT4_FEATURE_RO_COMPAT_SUPP)) {
		ext4_msg(sb, KERN_ERR, "couldn't mount RDWR because of "
			 "unsupported optional features (%x)",
			 (le32_to_cpu(EXT4_SB(sb)->s_es->s_feature_ro_compat) &
				~EXT4_FEATURE_RO_COMPAT_SUPP));
		return 0;
	}
	 
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb, EXT4_FEATURE_RO_COMPAT_HUGE_FILE)) {
		if (sizeof(blkcnt_t) < sizeof(u64)) {
			ext4_msg(sb, KERN_ERR, "Filesystem with huge files "
				 "cannot be mounted RDWR without "
				 "CONFIG_LBDAF");
			return 0;
		}
	}
	return 1;
}