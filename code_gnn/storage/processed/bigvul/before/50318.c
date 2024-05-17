void ceph_init_inode_acls(struct inode* inode, struct ceph_acls_info *info)
{
	if (!inode)
		return;
	ceph_set_cached_acl(inode, ACL_TYPE_ACCESS, info->acl);
	ceph_set_cached_acl(inode, ACL_TYPE_DEFAULT, info->default_acl);
}
