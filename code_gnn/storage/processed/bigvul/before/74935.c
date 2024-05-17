archive_acl_reset(struct archive_acl *acl, int want_type)
{
	int count, cutoff;

	count = archive_acl_count(acl, want_type);

	 
	if ((want_type & ARCHIVE_ENTRY_ACL_TYPE_ACCESS) != 0)
		cutoff = 3;
	else
		cutoff = 0;

	if (count > cutoff)
		acl->acl_state = ARCHIVE_ENTRY_ACL_USER_OBJ;
	else
		acl->acl_state = 0;
	acl->acl_p = acl->acl_head;
	return (count);
}
