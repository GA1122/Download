static void *jffs2_acl_to_medium(const struct posix_acl *acl, size_t *size)
{
	struct jffs2_acl_header *header;
	struct jffs2_acl_entry *entry;
	void *e;
	size_t i;

	*size = jffs2_acl_size(acl->a_count);
	header = kmalloc(sizeof(*header) + acl->a_count * sizeof(*entry), GFP_KERNEL);
	if (!header)
		return ERR_PTR(-ENOMEM);
	header->a_version = cpu_to_je32(JFFS2_ACL_VERSION);
	e = header + 1;
	for (i=0; i < acl->a_count; i++) {
		const struct posix_acl_entry *acl_e = &acl->a_entries[i];
		entry = e;
		entry->e_tag = cpu_to_je16(acl_e->e_tag);
		entry->e_perm = cpu_to_je16(acl_e->e_perm);
		switch(acl_e->e_tag) {
			case ACL_USER:
				entry->e_id = cpu_to_je32(
					from_kuid(&init_user_ns, acl_e->e_uid));
				e += sizeof(struct jffs2_acl_entry);
				break;
			case ACL_GROUP:
				entry->e_id = cpu_to_je32(
					from_kgid(&init_user_ns, acl_e->e_gid));
				e += sizeof(struct jffs2_acl_entry);
				break;

			case ACL_USER_OBJ:
			case ACL_GROUP_OBJ:
			case ACL_MASK:
			case ACL_OTHER:
				e += sizeof(struct jffs2_acl_entry_short);
				break;

			default:
				goto fail;
		}
	}
	return header;
 fail:
	kfree(header);
	return ERR_PTR(-EINVAL);
}
