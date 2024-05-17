static size_t jffs2_acl_size(int count)
{
	if (count <= 4) {
		return sizeof(struct jffs2_acl_header)
		       + count * sizeof(struct jffs2_acl_entry_short);
	} else {
		return sizeof(struct jffs2_acl_header)
		       + 4 * sizeof(struct jffs2_acl_entry_short)
		       + (count - 4) * sizeof(struct jffs2_acl_entry);
	}
}
