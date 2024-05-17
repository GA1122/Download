smb2_hdr_assemble(struct smb2_hdr *hdr, __le16 smb2_cmd   ,
		  const struct cifs_tcon *tcon)
{
	struct smb2_pdu *pdu = (struct smb2_pdu *)hdr;
	char *temp = (char *)hdr;
	 
	__u16 parmsize = smb2_req_struct_sizes[le16_to_cpu(smb2_cmd)];

	 
	memset(temp, 0, 256);

	 
	hdr->smb2_buf_length = cpu_to_be32(parmsize + sizeof(struct smb2_hdr)
			- 4  );

	hdr->ProtocolId[0] = 0xFE;
	hdr->ProtocolId[1] = 'S';
	hdr->ProtocolId[2] = 'M';
	hdr->ProtocolId[3] = 'B';
	hdr->StructureSize = cpu_to_le16(64);
	hdr->Command = smb2_cmd;
	hdr->CreditRequest = cpu_to_le16(2);  
	hdr->ProcessId = cpu_to_le32((__u16)current->tgid);

	if (!tcon)
		goto out;

	 
	 
	if ((tcon->ses) &&
	    (tcon->ses->server->capabilities & SMB2_GLOBAL_CAP_LARGE_MTU))
		hdr->CreditCharge = cpu_to_le16(1);
	 

	hdr->TreeId = tcon->tid;
	 
	if (tcon->ses)
		hdr->SessionId = tcon->ses->Suid;

	 
 

	if (tcon->ses && tcon->ses->server && tcon->ses->server->sign)
		hdr->Flags |= SMB2_FLAGS_SIGNED;
out:
	pdu->StructureSize2 = cpu_to_le16(parmsize);
	return;
}
