static struct posix_acl *jffs2_acl_from_medium(void *value, size_t size)
{
	void *end = value + size;
	struct jffs2_acl_header *header = value;
	struct jffs2_acl_entry *entry;
	struct posix_acl *acl;
	uint32_t ver;
	int i, count;

	if (!value)
		return NULL;
	if (size < sizeof(struct jffs2_acl_header))
		return ERR_PTR(-EINVAL);
	ver = je32_to_cpu(header->a_version);
	if (ver != JFFS2_ACL_VERSION) {
		JFFS2_WARNING("Invalid ACL version. (=%u)\n", ver);
		return ERR_PTR(-EINVAL);
	}

	value += sizeof(struct jffs2_acl_header);
	count = jffs2_acl_count(size);
	if (count < 0)
		return ERR_PTR(-EINVAL);
	if (count == 0)
		return NULL;

	acl = posix_acl_alloc(count, GFP_KERNEL);
	if (!acl)
		return ERR_PTR(-ENOMEM);

	for (i=0; i < count; i++) {
		entry = value;
		if (value + sizeof(struct jffs2_acl_entry_short) > end)
			goto fail;
		acl->a_entries[i].e_tag = je16_to_cpu(entry->e_tag);
		acl->a_entries[i].e_perm = je16_to_cpu(entry->e_perm);
		switch (acl->a_entries[i].e_tag) {
			case ACL_USER_OBJ:
			case ACL_GROUP_OBJ:
			case ACL_MASK:
			case ACL_OTHER:
				value += sizeof(struct jffs2_acl_entry_short);
				break;

			case ACL_USER:
				value += sizeof(struct jffs2_acl_entry);
				if (value > end)
					goto fail;
				acl->a_entries[i].e_uid =
					make_kuid(&init_user_ns,
						  je32_to_cpu(entry->e_id));
				break;
			case ACL_GROUP:
				value += sizeof(struct jffs2_acl_entry);
				if (value > end)
					goto fail;
				acl->a_entries[i].e_gid =
					make_kgid(&init_user_ns,
						  je32_to_cpu(entry->e_id));
				break;

			default:
				goto fail;
		}
	}
	if (value != end)
		goto fail;
	return acl;
 fail:
	posix_acl_release(acl);
	return ERR_PTR(-EINVAL);
}
