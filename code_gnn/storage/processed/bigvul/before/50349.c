static int jffs2_acl_count(size_t size)
{
	size_t s;

	size -= sizeof(struct jffs2_acl_header);
	if (size < 4 * sizeof(struct jffs2_acl_entry_short)) {
		if (size % sizeof(struct jffs2_acl_entry_short))
			return -1;
		return size / sizeof(struct jffs2_acl_entry_short);
	} else {
		s = size - 4 * sizeof(struct jffs2_acl_entry_short);
		if (s % sizeof(struct jffs2_acl_entry))
			return -1;
		return s / sizeof(struct jffs2_acl_entry) + 4;
	}
}
