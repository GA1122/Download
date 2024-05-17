static void __propagate_umount(struct mount *mnt)
{
	struct mount *parent = mnt->mnt_parent;
	struct mount *m;

	BUG_ON(parent == mnt);

	for (m = propagation_next(parent, parent); m;
			m = propagation_next(m, parent)) {

		struct mount *child = __lookup_mnt_last(&m->mnt,
						mnt->mnt_mountpoint);
		 
		if (!child || !IS_MNT_MARKED(child))
			continue;
		CLEAR_MNT_MARK(child);
		if (list_empty(&child->mnt_mounts)) {
			list_del_init(&child->mnt_child);
			child->mnt.mnt_flags |= MNT_UMOUNT;
			list_move_tail(&child->mnt_list, &mnt->mnt_list);
		}
	}
}