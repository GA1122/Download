static int unlazy_walk(struct nameidata *nd, struct dentry *dentry)
{
	struct fs_struct *fs = current->fs;
	struct dentry *parent = nd->path.dentry;

	BUG_ON(!(nd->flags & LOOKUP_RCU));

	 
	if (!legitimize_mnt(nd->path.mnt, nd->m_seq))
		return -ECHILD;
	nd->flags &= ~LOOKUP_RCU;

	if (!lockref_get_not_dead(&parent->d_lockref)) {
		nd->path.dentry = NULL;	
		goto out;
	}

	 
	if (!dentry) {
		if (read_seqcount_retry(&parent->d_seq, nd->seq))
			goto out;
		BUG_ON(nd->inode != parent->d_inode);
	} else {
		if (!lockref_get_not_dead(&dentry->d_lockref))
			goto out;
		if (read_seqcount_retry(&dentry->d_seq, nd->seq))
			goto drop_dentry;
	}

	 
	if (nd->root.mnt && !(nd->flags & LOOKUP_ROOT)) {
		spin_lock(&fs->lock);
		if (nd->root.mnt != fs->root.mnt || nd->root.dentry != fs->root.dentry)
			goto unlock_and_drop_dentry;
		path_get(&nd->root);
		spin_unlock(&fs->lock);
	}

	rcu_read_unlock();
	return 0;

unlock_and_drop_dentry:
	spin_unlock(&fs->lock);
drop_dentry:
	rcu_read_unlock();
	dput(dentry);
	goto drop_root_mnt;
out:
	rcu_read_unlock();
drop_root_mnt:
	if (!(nd->flags & LOOKUP_ROOT))
		nd->root.mnt = NULL;
	return -ECHILD;
}