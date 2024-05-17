id_mode_to_cifs_acl(struct inode *inode, const char *path, __u64 nmode,
			kuid_t uid, kgid_t gid)
{
	int rc = 0;
	int aclflag = CIFS_ACL_DACL;  
	__u32 secdesclen = 0;
	struct cifs_ntsd *pntsd = NULL;  
	struct cifs_ntsd *pnntsd = NULL;  
	struct cifs_sb_info *cifs_sb = CIFS_SB(inode->i_sb);
	struct tcon_link *tlink = cifs_sb_tlink(cifs_sb);
	struct cifs_tcon *tcon;

	if (IS_ERR(tlink))
		return PTR_ERR(tlink);
	tcon = tlink_tcon(tlink);

	cifs_dbg(NOISY, "set ACL from mode for %s\n", path);

	 

	if (tcon->ses->server->ops->get_acl == NULL) {
		cifs_put_tlink(tlink);
		return -EOPNOTSUPP;
	}

	pntsd = tcon->ses->server->ops->get_acl(cifs_sb, inode, path,
						&secdesclen);
	if (IS_ERR(pntsd)) {
		rc = PTR_ERR(pntsd);
		cifs_dbg(VFS, "%s: error %d getting sec desc\n", __func__, rc);
		cifs_put_tlink(tlink);
		return rc;
	}

	 
	secdesclen = max_t(u32, secdesclen, DEFAULT_SEC_DESC_LEN);
	pnntsd = kmalloc(secdesclen, GFP_KERNEL);
	if (!pnntsd) {
		kfree(pntsd);
		cifs_put_tlink(tlink);
		return -ENOMEM;
	}

	rc = build_sec_desc(pntsd, pnntsd, secdesclen, nmode, uid, gid,
				&aclflag);

	cifs_dbg(NOISY, "build_sec_desc rc: %d\n", rc);

	if (tcon->ses->server->ops->set_acl == NULL)
		rc = -EOPNOTSUPP;

	if (!rc) {
		 
		rc = tcon->ses->server->ops->set_acl(pnntsd, secdesclen, inode,
						     path, aclflag);
		cifs_dbg(NOISY, "set_cifs_acl rc: %d\n", rc);
	}
	cifs_put_tlink(tlink);

	kfree(pnntsd);
	kfree(pntsd);
	return rc;
}
