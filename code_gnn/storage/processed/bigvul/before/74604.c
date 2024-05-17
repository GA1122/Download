static int ovl_show_options(struct seq_file *m, struct dentry *dentry)
{
	struct super_block *sb = dentry->d_sb;
	struct ovl_fs *ufs = sb->s_fs_info;

	seq_printf(m, ",lowerdir=%s", ufs->config.lowerdir);
	seq_printf(m, ",upperdir=%s", ufs->config.upperdir);
	seq_printf(m, ",workdir=%s", ufs->config.workdir);
	return 0;
}