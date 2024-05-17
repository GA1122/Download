SMB2_rmdir(const unsigned int xid, struct cifs_tcon *tcon,
		  u64 persistent_fid, u64 volatile_fid)
{
	__u8 delete_pending = 1;
	void *data;
	unsigned int size;

	data = &delete_pending;
	size = 1;  

	return send_set_info(xid, tcon, persistent_fid, volatile_fid,
			current->tgid, FILE_DISPOSITION_INFORMATION, 1, &data,
			&size);
}