bool current_chrooted(void)
{
	 
	struct path ns_root;
	struct path fs_root;
	bool chrooted;

	 
	ns_root.mnt = &current->nsproxy->mnt_ns->root->mnt;
	ns_root.dentry = ns_root.mnt->mnt_root;
	path_get(&ns_root);
	while (d_mountpoint(ns_root.dentry) && follow_down_one(&ns_root))
		;

	get_fs_root(current->fs, &fs_root);

	chrooted = !path_equal(&fs_root, &ns_root);

	path_put(&fs_root);
	path_put(&ns_root);

	return chrooted;
}
