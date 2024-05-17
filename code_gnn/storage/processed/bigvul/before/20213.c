struct file *hugetlb_file_setup(const char *name, size_t size,
				vm_flags_t acctflag,
				struct user_struct **user, int creat_flags)
{
	int error = -ENOMEM;
	struct file *file;
	struct inode *inode;
	struct path path;
	struct dentry *root;
	struct qstr quick_string;

	*user = NULL;
	if (!hugetlbfs_vfsmount)
		return ERR_PTR(-ENOENT);

	if (creat_flags == HUGETLB_SHMFS_INODE && !can_do_hugetlb_shm()) {
		*user = current_user();
		if (user_shm_lock(size, *user)) {
			printk_once(KERN_WARNING "Using mlock ulimits for SHM_HUGETLB is deprecated\n");
		} else {
			*user = NULL;
			return ERR_PTR(-EPERM);
		}
	}

	root = hugetlbfs_vfsmount->mnt_root;
	quick_string.name = name;
	quick_string.len = strlen(quick_string.name);
	quick_string.hash = 0;
	path.dentry = d_alloc(root, &quick_string);
	if (!path.dentry)
		goto out_shm_unlock;

	path.mnt = mntget(hugetlbfs_vfsmount);
	error = -ENOSPC;
	inode = hugetlbfs_get_inode(root->d_sb, NULL, S_IFREG | S_IRWXUGO, 0);
	if (!inode)
		goto out_dentry;

	error = -ENOMEM;
	if (hugetlb_reserve_pages(inode, 0,
			size >> huge_page_shift(hstate_inode(inode)), NULL,
			acctflag))
		goto out_inode;

	d_instantiate(path.dentry, inode);
	inode->i_size = size;
	clear_nlink(inode);

	error = -ENFILE;
	file = alloc_file(&path, FMODE_WRITE | FMODE_READ,
			&hugetlbfs_file_operations);
	if (!file)
		goto out_dentry;  

	return file;

out_inode:
	iput(inode);
out_dentry:
	path_put(&path);
out_shm_unlock:
	if (*user) {
		user_shm_unlock(size, *user);
		*user = NULL;
	}
	return ERR_PTR(error);
}