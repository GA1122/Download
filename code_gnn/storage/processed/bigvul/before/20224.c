static struct inode *hugetlbfs_get_root(struct super_block *sb,
					struct hugetlbfs_config *config)
{
	struct inode *inode;

	inode = new_inode(sb);
	if (inode) {
		struct hugetlbfs_inode_info *info;
		inode->i_ino = get_next_ino();
		inode->i_mode = S_IFDIR | config->mode;
		inode->i_uid = config->uid;
		inode->i_gid = config->gid;
		inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
		info = HUGETLBFS_I(inode);
		mpol_shared_policy_init(&info->policy, NULL);
		inode->i_op = &hugetlbfs_dir_inode_operations;
		inode->i_fop = &simple_dir_operations;
		 
		inc_nlink(inode);
	}
	return inode;
}