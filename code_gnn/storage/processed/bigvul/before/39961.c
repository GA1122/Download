void cifsFileInfo_put(struct cifsFileInfo *cifs_file)
{
	struct inode *inode = cifs_file->dentry->d_inode;
	struct cifs_tcon *tcon = tlink_tcon(cifs_file->tlink);
	struct TCP_Server_Info *server = tcon->ses->server;
	struct cifsInodeInfo *cifsi = CIFS_I(inode);
	struct super_block *sb = inode->i_sb;
	struct cifs_sb_info *cifs_sb = CIFS_SB(sb);
	struct cifsLockInfo *li, *tmp;
	struct cifs_fid fid;
	struct cifs_pending_open open;

	spin_lock(&cifs_file_list_lock);
	if (--cifs_file->count > 0) {
		spin_unlock(&cifs_file_list_lock);
		return;
	}

	if (server->ops->get_lease_key)
		server->ops->get_lease_key(inode, &fid);

	 
	cifs_add_pending_open_locked(&fid, cifs_file->tlink, &open);

	 
	list_del(&cifs_file->flist);
	list_del(&cifs_file->tlist);

	if (list_empty(&cifsi->openFileList)) {
		cifs_dbg(FYI, "closing last open instance for inode %p\n",
			 cifs_file->dentry->d_inode);
		 
		if (cifs_sb->mnt_cifs_flags & CIFS_MOUNT_STRICT_IO)
			CIFS_I(inode)->invalid_mapping = true;
		cifs_set_oplock_level(cifsi, 0);
	}
	spin_unlock(&cifs_file_list_lock);

	cancel_work_sync(&cifs_file->oplock_break);

	if (!tcon->need_reconnect && !cifs_file->invalidHandle) {
		struct TCP_Server_Info *server = tcon->ses->server;
		unsigned int xid;

		xid = get_xid();
		if (server->ops->close)
			server->ops->close(xid, tcon, &cifs_file->fid);
		_free_xid(xid);
	}

	cifs_del_pending_open(&open);

	 
	down_write(&cifsi->lock_sem);
	list_for_each_entry_safe(li, tmp, &cifs_file->llist->locks, llist) {
		list_del(&li->llist);
		cifs_del_lock_waiters(li);
		kfree(li);
	}
	list_del(&cifs_file->llist->llist);
	kfree(cifs_file->llist);
	up_write(&cifsi->lock_sem);

	cifs_put_tlink(cifs_file->tlink);
	dput(cifs_file->dentry);
	cifs_sb_deactive(sb);
	kfree(cifs_file);
}
