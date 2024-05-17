create_durable_v2_buf(struct cifs_fid *pfid)
{
	struct create_durable_v2 *buf;

	buf = kzalloc(sizeof(struct create_durable_v2), GFP_KERNEL);
	if (!buf)
		return NULL;

	buf->ccontext.DataOffset = cpu_to_le16(offsetof
					(struct create_durable_v2, dcontext));
	buf->ccontext.DataLength = cpu_to_le32(sizeof(struct durable_context_v2));
	buf->ccontext.NameOffset = cpu_to_le16(offsetof
				(struct create_durable_v2, Name));
	buf->ccontext.NameLength = cpu_to_le16(4);

	buf->dcontext.Timeout = 0;  
	buf->dcontext.Flags = cpu_to_le32(SMB2_DHANDLE_FLAG_PERSISTENT);
	generate_random_uuid(buf->dcontext.CreateGuid);
	memcpy(pfid->create_guid, buf->dcontext.CreateGuid, 16);

	 
	buf->Name[0] = 'D';
	buf->Name[1] = 'H';
	buf->Name[2] = '2';
	buf->Name[3] = 'Q';
	return buf;
}