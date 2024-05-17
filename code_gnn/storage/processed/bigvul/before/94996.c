ext4_xattr_check_names(struct ext4_xattr_entry *entry, void *end,
		       void *value_start)
{
	struct ext4_xattr_entry *e = entry;

	while (!IS_LAST_ENTRY(e)) {
		struct ext4_xattr_entry *next = EXT4_XATTR_NEXT(e);
		if ((void *)next >= end)
			return -EFSCORRUPTED;
		e = next;
	}

	while (!IS_LAST_ENTRY(entry)) {
		if (entry->e_value_size != 0 &&
		    (value_start + le16_to_cpu(entry->e_value_offs) <
		     (void *)e + sizeof(__u32) ||
		     value_start + le16_to_cpu(entry->e_value_offs) +
		    le32_to_cpu(entry->e_value_size) > end))
			return -EFSCORRUPTED;
		entry = EXT4_XATTR_NEXT(entry);
	}

	return 0;
}
