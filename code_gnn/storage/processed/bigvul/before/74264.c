void dir_scan(char *parent_name, unsigned int start_block, unsigned int offset,
	struct pathnames *paths)
{
	unsigned int type;
	char *name;
	struct pathnames *new;
	struct inode *i;
	struct dir *dir = s_ops.squashfs_opendir(start_block, offset, &i);

	if(dir == NULL) {
		ERROR("dir_scan: failed to read directory %s, skipping\n",
			parent_name);
		return;
	}

	if(lsonly || info)
		print_filename(parent_name, i);

	if(!lsonly) {
		 
		int res = mkdir(parent_name, S_IRUSR|S_IWUSR|S_IXUSR);
		if(res == -1) {
			 
			if(!force || errno != EEXIST) {
				ERROR("dir_scan: failed to make directory %s, "
					"because %s\n", parent_name,
					strerror(errno));
				squashfs_closedir(dir);
				FAILED = TRUE;
				return;
			} 

			 
			res = chmod(parent_name, S_IRUSR|S_IWUSR|S_IXUSR);
			if (res == -1)
				ERROR("dir_scan: failed to change permissions "
					"for directory %s, because %s\n",
					parent_name, strerror(errno));
		}
	}

	while(squashfs_readdir(dir, &name, &start_block, &offset, &type)) {
		char *pathname;
		int res;

		TRACE("dir_scan: name %s, start_block %d, offset %d, type %d\n",
			name, start_block, offset, type);


		if(!matches(paths, name, &new))
			continue;

		res = asprintf(&pathname, "%s/%s", parent_name, name);
		if(res == -1)
			EXIT_UNSQUASH("asprintf failed in dir_scan\n");

		if(type == SQUASHFS_DIR_TYPE) {
			dir_scan(pathname, start_block, offset, new);
			free(pathname);
		} else if(new == NULL) {
			update_info(pathname);

			i = s_ops.read_inode(start_block, offset);

			if(lsonly || info)
				print_filename(pathname, i);

			if(!lsonly)
				create_inode(pathname, i);

			if(i->type == SQUASHFS_SYMLINK_TYPE ||
					i->type == SQUASHFS_LSYMLINK_TYPE)
				free(i->symlink);
		} else
			free(pathname);

		free_subdir(new);
	}

	if(!lsonly)
		queue_dir(parent_name, dir);

	squashfs_closedir(dir);
	dir_count ++;
}