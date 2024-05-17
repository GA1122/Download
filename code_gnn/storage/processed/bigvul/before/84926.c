SMB2_tcon(const unsigned int xid, struct cifs_ses *ses, const char *tree,
	  struct cifs_tcon *tcon, const struct nls_table *cp)
{
	struct smb2_tree_connect_req *req;
	struct smb2_tree_connect_rsp *rsp = NULL;
	struct kvec iov[2];
	struct kvec rsp_iov;
	int rc = 0;
	int resp_buftype;
	int unc_path_len;
	struct TCP_Server_Info *server;
	__le16 *unc_path = NULL;
	int flags = 0;

	cifs_dbg(FYI, "TCON\n");

	if ((ses->server) && tree)
		server = ses->server;
	else
		return -EIO;

	if (tcon && tcon->bad_network_name)
		return -ENOENT;

	if ((tcon && tcon->seal) &&
	    ((ses->server->capabilities & SMB2_GLOBAL_CAP_ENCRYPTION) == 0)) {
		cifs_dbg(VFS, "encryption requested but no server support");
		return -EOPNOTSUPP;
	}

	unc_path = kmalloc(MAX_SHARENAME_LENGTH * 2, GFP_KERNEL);
	if (unc_path == NULL)
		return -ENOMEM;

	unc_path_len = cifs_strtoUTF16(unc_path, tree, strlen(tree), cp) + 1;
	unc_path_len *= 2;
	if (unc_path_len < 2) {
		kfree(unc_path);
		return -EINVAL;
	}

	rc = small_smb2_init(SMB2_TREE_CONNECT, tcon, (void **) &req);
	if (rc) {
		kfree(unc_path);
		return rc;
	}

	if (ses->session_flags & SMB2_SESSION_FLAG_ENCRYPT_DATA)
		flags |= CIFS_TRANSFORM_REQ;

	if (tcon == NULL) {
		 
		req->hdr.sync_hdr.SessionId = ses->Suid;
		 
	}

	iov[0].iov_base = (char *)req;
	 
	iov[0].iov_len = get_rfc1002_length(req) + 4 - 1;

	 
	req->PathOffset = cpu_to_le16(sizeof(struct smb2_tree_connect_req)
			- 1   - 4  );
	req->PathLength = cpu_to_le16(unc_path_len - 2);
	iov[1].iov_base = unc_path;
	iov[1].iov_len = unc_path_len;

	inc_rfc1001_len(req, unc_path_len - 1  );

	rc = SendReceive2(xid, ses, iov, 2, &resp_buftype, flags, &rsp_iov);
	cifs_small_buf_release(req);
	rsp = (struct smb2_tree_connect_rsp *)rsp_iov.iov_base;

	if (rc != 0) {
		if (tcon) {
			cifs_stats_fail_inc(tcon, SMB2_TREE_CONNECT_HE);
			tcon->need_reconnect = true;
		}
		goto tcon_error_exit;
	}

	if (tcon == NULL) {
		ses->ipc_tid = rsp->hdr.sync_hdr.TreeId;
		goto tcon_exit;
	}

	if (rsp->ShareType & SMB2_SHARE_TYPE_DISK)
		cifs_dbg(FYI, "connection to disk share\n");
	else if (rsp->ShareType & SMB2_SHARE_TYPE_PIPE) {
		tcon->ipc = true;
		cifs_dbg(FYI, "connection to pipe share\n");
	} else if (rsp->ShareType & SMB2_SHARE_TYPE_PRINT) {
		tcon->print = true;
		cifs_dbg(FYI, "connection to printer\n");
	} else {
		cifs_dbg(VFS, "unknown share type %d\n", rsp->ShareType);
		rc = -EOPNOTSUPP;
		goto tcon_error_exit;
	}

	tcon->share_flags = le32_to_cpu(rsp->ShareFlags);
	tcon->capabilities = rsp->Capabilities;  
	tcon->maximal_access = le32_to_cpu(rsp->MaximalAccess);
	tcon->tidStatus = CifsGood;
	tcon->need_reconnect = false;
	tcon->tid = rsp->hdr.sync_hdr.TreeId;
	strlcpy(tcon->treeName, tree, sizeof(tcon->treeName));

	if ((rsp->Capabilities & SMB2_SHARE_CAP_DFS) &&
	    ((tcon->share_flags & SHI1005_FLAGS_DFS) == 0))
		cifs_dbg(VFS, "DFS capability contradicts DFS flag\n");
	init_copy_chunk_defaults(tcon);
	if (tcon->share_flags & SHI1005_FLAGS_ENCRYPT_DATA)
		cifs_dbg(VFS, "Encrypted shares not supported");
	if (tcon->ses->server->ops->validate_negotiate)
		rc = tcon->ses->server->ops->validate_negotiate(xid, tcon);
tcon_exit:
	free_rsp_buf(resp_buftype, rsp);
	kfree(unc_path);
	return rc;

tcon_error_exit:
	if (rsp->hdr.sync_hdr.Status == STATUS_BAD_NETWORK_NAME) {
		cifs_dbg(VFS, "BAD_NETWORK_NAME: %s\n", tree);
		if (tcon)
			tcon->bad_network_name = true;
	}
	goto tcon_exit;
}