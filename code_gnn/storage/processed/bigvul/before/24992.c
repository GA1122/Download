CIFSSMBTDis(const int xid, struct cifs_tcon *tcon)
{
	struct smb_hdr *smb_buffer;
	int rc = 0;

	cFYI(1, "In tree disconnect");

	 
	if ((tcon->ses == NULL) || (tcon->ses->server == NULL))
		return -EIO;

	 
	if ((tcon->need_reconnect) || (tcon->ses->need_reconnect))
		return 0;

	rc = small_smb_init(SMB_COM_TREE_DISCONNECT, 0, tcon,
			    (void **)&smb_buffer);
	if (rc)
		return rc;

	rc = SendReceiveNoRsp(xid, tcon->ses, smb_buffer, 0);
	if (rc)
		cFYI(1, "Tree disconnect failed %d", rc);

	 
	if (rc == -EAGAIN)
		rc = 0;

	return rc;
}
