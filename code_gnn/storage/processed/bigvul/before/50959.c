static bool mnt_already_visible(struct mnt_namespace *ns, struct vfsmount *new,
				int *new_mnt_flags)
{
	int new_flags = *new_mnt_flags;
	struct mount *mnt;
	bool visible = false;

	down_read(&namespace_sem);
	list_for_each_entry(mnt, &ns->list, mnt_list) {
		struct mount *child;
		int mnt_flags;

		if (mnt->mnt.mnt_sb->s_type != new->mnt_sb->s_type)
			continue;

		 
		if (mnt->mnt.mnt_root != mnt->mnt.mnt_sb->s_root)
			continue;

		 
		mnt_flags = mnt->mnt.mnt_flags;

		 
		if (mnt->mnt.mnt_sb->s_flags & MS_RDONLY)
			mnt_flags |= MNT_LOCK_READONLY;

		 
		if ((mnt_flags & MNT_LOCK_READONLY) &&
		    !(new_flags & MNT_READONLY))
			continue;
		if ((mnt_flags & MNT_LOCK_ATIME) &&
		    ((mnt_flags & MNT_ATIME_MASK) != (new_flags & MNT_ATIME_MASK)))
			continue;

		 
		list_for_each_entry(child, &mnt->mnt_mounts, mnt_child) {
			struct inode *inode = child->mnt_mountpoint->d_inode;
			 
			if (!(child->mnt.mnt_flags & MNT_LOCKED))
				continue;
			 
			if (!is_empty_dir_inode(inode))
				goto next;
		}
		 
		*new_mnt_flags |= mnt_flags & (MNT_LOCK_READONLY | \
					       MNT_LOCK_ATIME);
		visible = true;
		goto found;
	next:	;
	}
found:
	up_read(&namespace_sem);
	return visible;
}
