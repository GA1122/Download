smb2_hdr_assemble(struct smb2_sync_hdr *shdr, __le16 smb2_cmd,
		  const struct cifs_tcon *tcon)
{
	shdr->ProtocolId = SMB2_PROTO_NUMBER;
	shdr->StructureSize = cpu_to_le16(64);
	shdr->Command = smb2_cmd;
	if (tcon && tcon->ses && tcon->ses->server) {
		struct TCP_Server_Info *server = tcon->ses->server;

		spin_lock(&server->req_lock);
		 
		if (server->credits >= server->max_credits)
			shdr->CreditRequest = cpu_to_le16(0);
		else
			shdr->CreditRequest = cpu_to_le16(
				min_t(int, server->max_credits -
						server->credits, 2));
		spin_unlock(&server->req_lock);
	} else {
		shdr->CreditRequest = cpu_to_le16(2);
	}
	shdr->ProcessId = cpu_to_le32((__u16)current->tgid);

	if (!tcon)
		goto out;

	 
	 
	if ((tcon->ses) && (tcon->ses->server) &&
	    (tcon->ses->server->capabilities & SMB2_GLOBAL_CAP_LARGE_MTU))
		shdr->CreditCharge = cpu_to_le16(1);
	 

	shdr->TreeId = tcon->tid;
	 
	if (tcon->ses)
		shdr->SessionId = tcon->ses->Suid;

	 
 

	if (tcon->ses && tcon->ses->server && tcon->ses->server->sign &&
	    !encryption_required(tcon))
		shdr->Flags |= SMB2_FLAGS_SIGNED;
out:
	return;
}
