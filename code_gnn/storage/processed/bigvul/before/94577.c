static void __d_move(struct dentry *dentry, struct dentry *target,
		     bool exchange)
{
	if (!dentry->d_inode)
		printk(KERN_WARNING "VFS: moving negative dcache entry\n");

	BUG_ON(d_ancestor(dentry, target));
	BUG_ON(d_ancestor(target, dentry));

	dentry_lock_for_move(dentry, target);

	write_seqcount_begin(&dentry->d_seq);
	write_seqcount_begin_nested(&target->d_seq, DENTRY_D_LOCK_NESTED);

	 

	 
	__d_drop(dentry);
	__d_rehash(dentry, d_hash(target->d_parent, target->d_name.hash));

	 
	__d_drop(target);
	if (exchange) {
		__d_rehash(target,
			   d_hash(dentry->d_parent, dentry->d_name.hash));
	}

	 
	if (exchange)
		swap_names(dentry, target);
	else
		copy_name(dentry, target);

	 
	if (IS_ROOT(dentry)) {
		 
		dentry->d_parent = target->d_parent;
		target->d_parent = target;
		list_del_init(&target->d_child);
		list_move(&dentry->d_child, &dentry->d_parent->d_subdirs);
	} else {
		 
		swap(dentry->d_parent, target->d_parent);
		list_move(&target->d_child, &target->d_parent->d_subdirs);
		list_move(&dentry->d_child, &dentry->d_parent->d_subdirs);
		if (exchange)
			fsnotify_d_move(target);
		fsnotify_d_move(dentry);
	}

	write_seqcount_end(&target->d_seq);
	write_seqcount_end(&dentry->d_seq);

	dentry_unlock_for_move(dentry, target);
}