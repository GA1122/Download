static void *ocfs2_acl_to_xattr(const struct posix_acl *acl, size_t *size)
{
	struct ocfs2_acl_entry *entry = NULL;
	char *ocfs2_acl;
	size_t n;

	*size = acl->a_count * sizeof(struct posix_acl_entry);

	ocfs2_acl = kmalloc(*size, GFP_NOFS);
	if (!ocfs2_acl)
		return ERR_PTR(-ENOMEM);

	entry = (struct ocfs2_acl_entry *)ocfs2_acl;
	for (n = 0; n < acl->a_count; n++, entry++) {
		entry->e_tag  = cpu_to_le16(acl->a_entries[n].e_tag);
		entry->e_perm = cpu_to_le16(acl->a_entries[n].e_perm);
		switch(acl->a_entries[n].e_tag) {
		case ACL_USER:
			entry->e_id = cpu_to_le32(
				from_kuid(&init_user_ns,
					  acl->a_entries[n].e_uid));
			break;
		case ACL_GROUP:
			entry->e_id = cpu_to_le32(
				from_kgid(&init_user_ns,
					  acl->a_entries[n].e_gid));
			break;
		default:
			entry->e_id = cpu_to_le32(ACL_UNDEFINED_ID);
			break;
		}
	}
	return ocfs2_acl;
}
