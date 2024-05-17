xfs_acl_from_disk(
	const struct xfs_acl	*aclp,
	int			len,
	int			max_entries)
{
	struct posix_acl_entry *acl_e;
	struct posix_acl *acl;
	const struct xfs_acl_entry *ace;
	unsigned int count, i;

	if (len < sizeof(*aclp))
		return ERR_PTR(-EFSCORRUPTED);
	count = be32_to_cpu(aclp->acl_cnt);
	if (count > max_entries || XFS_ACL_SIZE(count) != len)
		return ERR_PTR(-EFSCORRUPTED);

	acl = posix_acl_alloc(count, GFP_KERNEL);
	if (!acl)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < count; i++) {
		acl_e = &acl->a_entries[i];
		ace = &aclp->acl_entry[i];

		 
		acl_e->e_tag = be32_to_cpu(ace->ae_tag);
		acl_e->e_perm = be16_to_cpu(ace->ae_perm);

		switch (acl_e->e_tag) {
		case ACL_USER:
			acl_e->e_uid = xfs_uid_to_kuid(be32_to_cpu(ace->ae_id));
			break;
		case ACL_GROUP:
			acl_e->e_gid = xfs_gid_to_kgid(be32_to_cpu(ace->ae_id));
			break;
		case ACL_USER_OBJ:
		case ACL_GROUP_OBJ:
		case ACL_MASK:
		case ACL_OTHER:
			break;
		default:
			goto fail;
		}
	}
	return acl;

fail:
	posix_acl_release(acl);
	return ERR_PTR(-EINVAL);
}