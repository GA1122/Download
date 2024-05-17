cifs_mount(struct cifs_sb_info *cifs_sb, struct smb_vol *volume_info)
{
	int rc;
	unsigned int xid;
	struct cifs_ses *ses;
	struct cifs_tcon *tcon;
	struct TCP_Server_Info *server;
	char   *full_path;
	struct tcon_link *tlink;
#ifdef CONFIG_CIFS_DFS_UPCALL
	int referral_walks_count = 0;
#endif

	rc = bdi_setup_and_register(&cifs_sb->bdi, "cifs", BDI_CAP_MAP_COPY);
	if (rc)
		return rc;

#ifdef CONFIG_CIFS_DFS_UPCALL
try_mount_again:
	 
	if (referral_walks_count) {
		if (tcon)
			cifs_put_tcon(tcon);
		else if (ses)
			cifs_put_smb_ses(ses);

		free_xid(xid);
	}
#endif
	rc = 0;
	tcon = NULL;
	ses = NULL;
	server = NULL;
	full_path = NULL;
	tlink = NULL;

	xid = get_xid();

	 
	server = cifs_get_tcp_session(volume_info);
	if (IS_ERR(server)) {
		rc = PTR_ERR(server);
		bdi_destroy(&cifs_sb->bdi);
		goto out;
	}

	 
	ses = cifs_get_smb_ses(server, volume_info);
	if (IS_ERR(ses)) {
		rc = PTR_ERR(ses);
		ses = NULL;
		goto mount_fail_check;
	}

	 
	tcon = cifs_get_tcon(ses, volume_info);
	if (IS_ERR(tcon)) {
		rc = PTR_ERR(tcon);
		tcon = NULL;
		goto remote_path_check;
	}

	 
	if (cap_unix(tcon->ses)) {
		 
		reset_cifs_unix_caps(xid, tcon, cifs_sb, volume_info);
		if ((tcon->ses->server->tcpStatus == CifsNeedReconnect) &&
		    (le64_to_cpu(tcon->fsUnixInfo.Capability) &
		     CIFS_UNIX_TRANSPORT_ENCRYPTION_MANDATORY_CAP)) {
			rc = -EACCES;
			goto mount_fail_check;
		}
	} else
		tcon->unix_ext = 0;  

	 
	if (!tcon->ipc && server->ops->qfs_tcon)
		server->ops->qfs_tcon(xid, tcon);

	cifs_sb->wsize = server->ops->negotiate_wsize(tcon, volume_info);
	cifs_sb->rsize = server->ops->negotiate_rsize(tcon, volume_info);

	 
	cifs_sb->bdi.ra_pages = cifs_sb->rsize / PAGE_CACHE_SIZE;

remote_path_check:
#ifdef CONFIG_CIFS_DFS_UPCALL
	 
	if (referral_walks_count == 0) {
		int refrc = expand_dfs_referral(xid, ses, volume_info, cifs_sb,
						false);
		if (!refrc) {
			referral_walks_count++;
			goto try_mount_again;
		}
	}
#endif

	 
	if (!rc && tcon) {
		if (!server->ops->is_path_accessible) {
			rc = -ENOSYS;
			goto mount_fail_check;
		}
		 
		full_path = cifs_build_path_to_root(volume_info, cifs_sb, tcon);
		if (full_path == NULL) {
			rc = -ENOMEM;
			goto mount_fail_check;
		}
		rc = server->ops->is_path_accessible(xid, tcon, cifs_sb,
						     full_path);
		if (rc != 0 && rc != -EREMOTE) {
			kfree(full_path);
			goto mount_fail_check;
		}
		kfree(full_path);
	}

	 
	if (rc == -EREMOTE) {
#ifdef CONFIG_CIFS_DFS_UPCALL
		if (referral_walks_count > MAX_NESTED_LINKS) {
			 
			rc = -ELOOP;
			goto mount_fail_check;
		}

		rc = expand_dfs_referral(xid, ses, volume_info, cifs_sb, true);

		if (!rc) {
			referral_walks_count++;
			goto try_mount_again;
		}
		goto mount_fail_check;
#else  
		rc = -EOPNOTSUPP;
#endif
	}

	if (rc)
		goto mount_fail_check;

	 
	tlink = kzalloc(sizeof *tlink, GFP_KERNEL);
	if (tlink == NULL) {
		rc = -ENOMEM;
		goto mount_fail_check;
	}

	tlink->tl_uid = ses->linux_uid;
	tlink->tl_tcon = tcon;
	tlink->tl_time = jiffies;
	set_bit(TCON_LINK_MASTER, &tlink->tl_flags);
	set_bit(TCON_LINK_IN_TREE, &tlink->tl_flags);

	cifs_sb->master_tlink = tlink;
	spin_lock(&cifs_sb->tlink_tree_lock);
	tlink_rb_insert(&cifs_sb->tlink_tree, tlink);
	spin_unlock(&cifs_sb->tlink_tree_lock);

	queue_delayed_work(cifsiod_wq, &cifs_sb->prune_tlinks,
				TLINK_IDLE_EXPIRE);

mount_fail_check:
	 
	if (rc) {
		 
		 
		if (tcon)
			cifs_put_tcon(tcon);
		else if (ses)
			cifs_put_smb_ses(ses);
		else
			cifs_put_tcp_session(server);
		bdi_destroy(&cifs_sb->bdi);
	}

out:
	free_xid(xid);
	return rc;
}
