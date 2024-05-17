static void umount_tree(struct mount *mnt, enum umount_tree_flags how)
{
	LIST_HEAD(tmp_list);
	struct mount *p;

	if (how & UMOUNT_PROPAGATE)
		propagate_mount_unlock(mnt);

	 
	for (p = mnt; p; p = next_mnt(p, mnt)) {
		p->mnt.mnt_flags |= MNT_UMOUNT;
		list_move(&p->mnt_list, &tmp_list);
	}

	 
	list_for_each_entry(p, &tmp_list, mnt_list) {
		list_del_init(&p->mnt_child);
	}

	 
	if (how & UMOUNT_PROPAGATE)
		propagate_umount(&tmp_list);

	while (!list_empty(&tmp_list)) {
		p = list_first_entry(&tmp_list, struct mount, mnt_list);
		list_del_init(&p->mnt_expire);
		list_del_init(&p->mnt_list);
		__touch_mnt_namespace(p->mnt_ns);
		p->mnt_ns = NULL;
		if (how & UMOUNT_SYNC)
			p->mnt.mnt_flags |= MNT_SYNC_UMOUNT;

		pin_insert_group(&p->mnt_umount, &p->mnt_parent->mnt, &unmounted);
		if (mnt_has_parent(p)) {
			hlist_del_init(&p->mnt_mp_list);
			put_mountpoint(p->mnt_mp);
			mnt_add_count(p->mnt_parent, -1);
			 
			p->mnt_ex_mountpoint = p->mnt_mountpoint;
			p->mnt_mountpoint = p->mnt.mnt_root;
			p->mnt_parent = p;
			p->mnt_mp = NULL;
			hlist_del_init_rcu(&p->mnt_hash);
		}
		change_mnt_propagation(p, MS_PRIVATE);
	}
}