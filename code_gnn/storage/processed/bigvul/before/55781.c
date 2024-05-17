sort_pacl(struct posix_acl *pacl)
{
	 
	int i, j;

	 
	if (!pacl || pacl->a_count <= 4)
		return;

	i = 1;
	while (pacl->a_entries[i].e_tag == ACL_USER)
		i++;
	sort_pacl_range(pacl, 1, i-1);

	BUG_ON(pacl->a_entries[i].e_tag != ACL_GROUP_OBJ);
	j = ++i;
	while (pacl->a_entries[j].e_tag == ACL_GROUP)
		j++;
	sort_pacl_range(pacl, i, j-1);
	return;
}