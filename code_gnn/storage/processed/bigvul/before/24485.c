cifs_get_tcon(struct cifsSesInfo *ses, struct smb_vol *volume_info)
{
	int rc, xid;
	struct cifsTconInfo *tcon;

	tcon = cifs_find_tcon(ses, volume_info->UNC);
	if (tcon) {
		cFYI(1, "Found match on UNC path");
		 
		cifs_put_smb_ses(ses);
		if (tcon->seal != volume_info->seal)
			cERROR(1, "transport encryption setting "
				   "conflicts with existing tid");
		return tcon;
	}

	tcon = tconInfoAlloc();
	if (tcon == NULL) {
		rc = -ENOMEM;
		goto out_fail;
	}

	tcon->ses = ses;
	if (volume_info->password) {
		tcon->password = kstrdup(volume_info->password, GFP_KERNEL);
		if (!tcon->password) {
			rc = -ENOMEM;
			goto out_fail;
		}
	}

	if (strchr(volume_info->UNC + 3, '\\') == NULL
	    && strchr(volume_info->UNC + 3, '/') == NULL) {
		cERROR(1, "Missing share name");
		rc = -ENODEV;
		goto out_fail;
	}

	 
	xid = GetXid();
	rc = CIFSTCon(xid, ses, volume_info->UNC, tcon, volume_info->local_nls);
	FreeXid(xid);
	cFYI(1, "CIFS Tcon rc = %d", rc);
	if (rc)
		goto out_fail;

	if (volume_info->nodfs) {
		tcon->Flags &= ~SMB_SHARE_IS_IN_DFS;
		cFYI(1, "DFS disabled (%d)", tcon->Flags);
	}
	tcon->seal = volume_info->seal;
	 
	tcon->retry = volume_info->retry;
	tcon->nocase = volume_info->nocase;
	tcon->local_lease = volume_info->local_lease;

	spin_lock(&cifs_tcp_ses_lock);
	list_add(&tcon->tcon_list, &ses->tcon_list);
	spin_unlock(&cifs_tcp_ses_lock);

	cifs_fscache_get_super_cookie(tcon);

	return tcon;

out_fail:
	tconInfoFree(tcon);
	return ERR_PTR(rc);
}