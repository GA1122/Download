static void cifs_convert_ace(posix_acl_xattr_entry *ace,
			     struct cifs_posix_ace *cifs_ace)
{
	 
	ace->e_perm = cpu_to_le16(cifs_ace->cifs_e_perm);
	ace->e_tag  = cpu_to_le16(cifs_ace->cifs_e_tag);
	ace->e_id   = cpu_to_le32(le64_to_cpu(cifs_ace->cifs_uid));
	 

	return;
}