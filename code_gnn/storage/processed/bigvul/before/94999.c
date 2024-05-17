ext4_xattr_find_entry(struct ext4_xattr_entry **pentry, int name_index,
		      const char *name, size_t size, int sorted)
{
	struct ext4_xattr_entry *entry;
	size_t name_len;
	int cmp = 1;

	if (name == NULL)
		return -EINVAL;
	name_len = strlen(name);
	entry = *pentry;
	for (; !IS_LAST_ENTRY(entry); entry = EXT4_XATTR_NEXT(entry)) {
		cmp = name_index - entry->e_name_index;
		if (!cmp)
			cmp = name_len - entry->e_name_len;
		if (!cmp)
			cmp = memcmp(name, entry->e_name, name_len);
		if (cmp <= 0 && (sorted || cmp == 0))
			break;
	}
	*pentry = entry;
	if (!cmp && ext4_xattr_check_entry(entry, size))
		return -EFSCORRUPTED;
	return cmp ? -ENODATA : 0;
}
