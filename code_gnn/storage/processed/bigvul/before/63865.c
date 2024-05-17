static int f2fs_freeze(struct super_block *sb)
{
	if (f2fs_readonly(sb))
		return 0;

	 
	if (unlikely(f2fs_cp_error(F2FS_SB(sb))))
		return -EIO;

	 
	if (is_sbi_flag_set(F2FS_SB(sb), SBI_IS_DIRTY))
		return -EINVAL;
	return 0;
}