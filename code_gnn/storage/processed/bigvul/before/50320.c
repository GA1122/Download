void ceph_release_acls_info(struct ceph_acls_info *info)
{
	posix_acl_release(info->acl);
	posix_acl_release(info->default_acl);
	if (info->pagelist)
		ceph_pagelist_release(info->pagelist);
}
