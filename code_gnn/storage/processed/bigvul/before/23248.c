static void nfs4_write_cached_acl(struct inode *inode, const char *buf, size_t acl_len)
{
	struct nfs4_cached_acl *acl;

	if (buf && acl_len <= PAGE_SIZE) {
		acl = kmalloc(sizeof(*acl) + acl_len, GFP_KERNEL);
		if (acl == NULL)
			goto out;
		acl->cached = 1;
		memcpy(acl->data, buf, acl_len);
	} else {
		acl = kmalloc(sizeof(*acl), GFP_KERNEL);
		if (acl == NULL)
			goto out;
		acl->cached = 0;
	}
	acl->len = acl_len;
out:
 	nfs4_set_cached_acl(inode, acl);
 }