 cifs_mount(struct super_block *sb, struct cifs_sb_info *cifs_sb,
 		char *mount_data_global, const char *devname)
 {
 	int rc;
 	int xid;
 	struct smb_vol *volume_info;
 	struct cifsSesInfo *pSesInfo;
 	struct cifsTconInfo *tcon;
 	struct TCP_Server_Info *srvTcp;
 	char   *full_path;
 	char *mount_data = mount_data_global;
 	struct tcon_link *tlink;
 #ifdef CONFIG_CIFS_DFS_UPCALL
 	struct dfs_info3_param *referrals = NULL;
 	unsigned int num_referrals = 0;
 	int referral_walks_count = 0;
 try_mount_again:
 #endif
 	rc = 0;
 	tcon = NULL;
 	pSesInfo = NULL;
 	srvTcp = NULL;
 	full_path = NULL;
 	tlink = NULL;
 
 	xid = GetXid();
 
 	volume_info = kzalloc(sizeof(struct smb_vol), GFP_KERNEL);
 	if (!volume_info) {
 		rc = -ENOMEM;
 		goto out;
 	}
 
 	if (cifs_parse_mount_options(mount_data, devname, volume_info)) {
 		rc = -EINVAL;
 		goto out;
 	}
 
 	if (volume_info->nullauth) {
 		cFYI(1, "null user");
 		volume_info->username = "";
 	} else if (volume_info->username) {
 		 
 		cFYI(1, "Username: %s", volume_info->username);
 	} else {
 		cifserror("No username specified");
 	 
 		rc = -EINVAL;
 		goto out;
 	}
 
 	 
 	if (volume_info->iocharset == NULL) {
 		 
 		volume_info->local_nls = load_nls_default();
 	} else {
 		volume_info->local_nls = load_nls(volume_info->iocharset);
 		if (volume_info->local_nls == NULL) {
 			cERROR(1, "CIFS mount error: iocharset %s not found",
 				 volume_info->iocharset);
 			rc = -ELIBACC;
 			goto out;
 		}
 	}
 	cifs_sb->local_nls = volume_info->local_nls;
 
 	 
 	srvTcp = cifs_get_tcp_session(volume_info);
 	if (IS_ERR(srvTcp)) {
 		rc = PTR_ERR(srvTcp);
 		goto out;
 	}
 
 	 
 	pSesInfo = cifs_get_smb_ses(srvTcp, volume_info);
 	if (IS_ERR(pSesInfo)) {
 		rc = PTR_ERR(pSesInfo);
 		pSesInfo = NULL;
 		goto mount_fail_check;
 	}
 
 	setup_cifs_sb(volume_info, cifs_sb);
 	if (pSesInfo->capabilities & CAP_LARGE_FILES)
 		sb->s_maxbytes = MAX_LFS_FILESIZE;
 	else
 		sb->s_maxbytes = MAX_NON_LFS;
 
 	 
 	sb->s_time_gran = 100;
 
 	 
 	tcon = cifs_get_tcon(pSesInfo, volume_info);
 	if (IS_ERR(tcon)) {
 		rc = PTR_ERR(tcon);
 		tcon = NULL;
 		goto remote_path_check;
 	}
 
 	 
 	if (!tcon->ipc) {
 		CIFSSMBQFSDeviceInfo(xid, tcon);
 		CIFSSMBQFSAttributeInfo(xid, tcon);
 	}
 
 	 
 	if (tcon->ses->capabilities & CAP_UNIX)
 		 
 		reset_cifs_unix_caps(xid, tcon, sb, volume_info);
 	else
 		tcon->unix_ext = 0;  
 
 	 
 	if ((cifs_sb->mnt_cifs_flags & CIFS_MOUNT_POSIX_PATHS) == 0)
 		convert_delimiter(cifs_sb->prepath, CIFS_DIR_SEP(cifs_sb));
 
 	if ((tcon->unix_ext == 0) && (cifs_sb->rsize > (1024 * 127))) {
 		cifs_sb->rsize = 1024 * 127;
 		cFYI(DBG2, "no very large read support, rsize now 127K");
 	}
 	if (!(tcon->ses->capabilities & CAP_LARGE_WRITE_X))
 		cifs_sb->wsize = min(cifs_sb->wsize,
 			       (tcon->ses->server->maxBuf - MAX_CIFS_HDR_SIZE));
 	if (!(tcon->ses->capabilities & CAP_LARGE_READ_X))
 		cifs_sb->rsize = min(cifs_sb->rsize,
 			       (tcon->ses->server->maxBuf - MAX_CIFS_HDR_SIZE));
  
  remote_path_check:
  	 
	if (!rc && cifs_sb->prepathlen && tcon) {
// 	if (!rc && tcon) {
  		 
  		full_path = cifs_build_path_to_root(cifs_sb, tcon);
  		if (full_path == NULL) {
 			rc = -ENOMEM;
 			goto mount_fail_check;
 		}
 		rc = is_path_accessible(xid, tcon, cifs_sb, full_path);
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
 		 
 		if ((cifs_sb->mnt_cifs_flags & CIFS_MOUNT_POSIX_PATHS) == 0)
 			convert_delimiter(cifs_sb->prepath,
 					CIFS_DIR_SEP(cifs_sb));
 		full_path = build_unc_path_to_root(volume_info, cifs_sb);
 		if (IS_ERR(full_path)) {
 			rc = PTR_ERR(full_path);
 			goto mount_fail_check;
 		}
 
 		cFYI(1, "Getting referral for: %s", full_path);
 		rc = get_dfs_path(xid, pSesInfo , full_path + 1,
 			cifs_sb->local_nls, &num_referrals, &referrals,
 			cifs_sb->mnt_cifs_flags & CIFS_MOUNT_MAP_SPECIAL_CHR);
 		if (!rc && num_referrals > 0) {
 			char *fake_devname = NULL;
 
 			if (mount_data != mount_data_global)
 				kfree(mount_data);
 
 			mount_data = cifs_compose_mount_options(
 					cifs_sb->mountdata, full_path + 1,
 					referrals, &fake_devname);
 
 			free_dfs_info_array(referrals, num_referrals);
 			kfree(fake_devname);
 			kfree(full_path);
 
 			if (IS_ERR(mount_data)) {
 				rc = PTR_ERR(mount_data);
 				mount_data = NULL;
 				goto mount_fail_check;
 			}
 
 			if (tcon)
 				cifs_put_tcon(tcon);
 			else if (pSesInfo)
 				cifs_put_smb_ses(pSesInfo);
 
 			cleanup_volume_info(&volume_info);
 			referral_walks_count++;
 			FreeXid(xid);
 			goto try_mount_again;
 		}
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
 
 	tlink->tl_uid = pSesInfo->linux_uid;
 	tlink->tl_tcon = tcon;
 	tlink->tl_time = jiffies;
 	set_bit(TCON_LINK_MASTER, &tlink->tl_flags);
 	set_bit(TCON_LINK_IN_TREE, &tlink->tl_flags);
 
 	cifs_sb->master_tlink = tlink;
 	spin_lock(&cifs_sb->tlink_tree_lock);
 	tlink_rb_insert(&cifs_sb->tlink_tree, tlink);
 	spin_unlock(&cifs_sb->tlink_tree_lock);
 
 	queue_delayed_work(system_nrt_wq, &cifs_sb->prune_tlinks,
 				TLINK_IDLE_EXPIRE);
 
 mount_fail_check:
 	 
 	if (rc) {
 		if (mount_data != mount_data_global)
 			kfree(mount_data);
 		 
 		 
 		if (tcon)
 			cifs_put_tcon(tcon);
 		else if (pSesInfo)
 			cifs_put_smb_ses(pSesInfo);
 		else
 			cifs_put_tcp_session(srvTcp);
 		goto out;
 	}
 
 	 
 out:
 	 
 	cleanup_volume_info(&volume_info);
 	FreeXid(xid);
 	return rc;
 }